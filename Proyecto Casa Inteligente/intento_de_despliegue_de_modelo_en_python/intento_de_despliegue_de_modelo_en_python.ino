#include <Arduino_LSM9DS1.h>
#include <PDM.h>
#include <TinyMLShield.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

#include "modelo3Miniproyecto.h"

const float tamanoVentana = 2.5;
const int numSamples = 124;
bool record = false;
bool commandRecv = false;
int samplesRead = numSamples;

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// array to map gesture index to a name
const char* GESTURES[] = {
  "AIRE",
  "COCINA",
  "COMANDO",
  "LUZ",
  "PATIO",
  "SALA",
  "TODAS",
  "ruido_fondo"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Initialize the TinyML Shield
  initializeShield();

  PDM.onReceive(onPDMdata);
  // Initialize PDM microphone in mono mode with 16 kHz sample rate
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM");
    while (1);
  }

  Serial.println("Welcome to the microphone test for the built-in microphone on the Nano 33 BLE Sense\n");
  Serial.println("Use the on-shield button or send the command 'click' to start and stop an audio recording");
  Serial.println("Open the Serial Plotter to view the corresponding waveform");
}
  Serial.println();
  
  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;
  bool clicked = readShieldButton();
  if (clicked){
    record = !record;
  }
  
  // see if a command was sent over the serial monitor and record it if so
  String command;
  while (Serial.available()) {
    char c = Serial.read();
    if ((c != '\n') && (c != '\r')) {
      command.concat(c);
    } 
    else if (c == '\r') {
      commandRecv = true;
      command.toLowerCase();
    }
  }
  while (samplesRead == numSamples) {
  // parse the command if applicable
  if (commandRecv && command == "click") {
    if (aSum >= tamanoVentana){
      
      samplesRead = 0;
      break;
      commandRecv = false;
      record = !record;
  }
}
}

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {

      // normalize the IMU data between 0 to 1 and store in the model's
      // input tensor
      tflInputTensor->data.f[samplesRead * 12 + 0] = (aX + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 12 + 1] = (aY + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 12 + 2] = (aZ + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 12 + 3] = (aX + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 4] = (aY + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 5] = (aZ + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 6] = (aX + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 7] = (aY + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 8] = (aZ + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 9] = (aX + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 10] = (aY + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 11] = (aZ + 100.0) / 2000.0;
      tflInputTensor->data.f[samplesRead * 12 + 12] = (aX + 100.0) / 2000.0;

      samplesRead++;

      if (samplesRead == numSamples) {
        // Run inferencing
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();
        if (invokeStatus != kTfLiteOk) {
          Serial.println("Invoke failed!");
          while (1);
          return;
        }

        // Loop through the output tensor values from the model
        for (int i = 0; i < NUM_GESTURES; i++) {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 6);
        }
        Serial.println();
      }
    }
  }
void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read data into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  samplesRead = bytesAvailable / 2;
}
