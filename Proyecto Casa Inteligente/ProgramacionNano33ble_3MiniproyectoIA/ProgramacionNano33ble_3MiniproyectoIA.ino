/* Edge Impulse ingestion SDK
 * Copyright (c) 2022 EdgeImpulse Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// If your target is limited in memory remove this macro to save 10K RAM
#define EIDSP_QUANTIZE_FILTERBANK   0

/**
 * Define the number of slices per model window. E.g. a model window of 1000 ms
 * with slices per model window set to 4. Results in a slice size of 250 ms.
 * For more info: https://docs.edgeimpulse.com/docs/continuous-audio-sampling
 */
#define EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW 3

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

/* Includes ---------------------------------------------------------------- */
#include <PDM.h>
#include <audioVisionProyectoVS5_inferencing.h>
/** Audio buffers, pointers and selectors */
int Detener = 0;
int Comando = 0;
int orden = D10;
int aire = D9;
int luz = D8;
int sala = D7;
int patio = D6;
int cocina = D5;
int todas = D4;
const int desbloquearAire = D3;
const int desbloquearLuces = D2;
int encenderAire = 0;
int encenderLuces = 0;
int seleccionLuz = 0;
int apagarAire = 0;
int apagarLuz = 0;
int apagarSala = 0;
int apagarCocina = 0;
int apagarPatio = 0;
int contadorLuces = LOW;
int contadorAire = LOW;
typedef struct {
    signed short *buffers[2];
    unsigned char buf_select;
    unsigned char buf_ready;
    unsigned int buf_count;
    unsigned int n_samples;
} inference_t;

static inference_t inference;
static bool record_ready = false;
static signed short *sampleBuffer;
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);

/**
 * @brief      Arduino setup function
 */
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

// *****************
// Lo Nuevo
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    pinMode(orden, OUTPUT);
    pinMode(aire, OUTPUT);
    pinMode(luz, OUTPUT);
    pinMode(sala, OUTPUT);
    pinMode(patio, OUTPUT);
    pinMode(cocina, OUTPUT);
    pinMode(todas, OUTPUT);
    pinMode(desbloquearAire, INPUT);
    pinMode(desbloquearLuces, INPUT);
    // Ensure the LED is off by default.
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(orden, LOW);
    digitalWrite(aire, LOW);
    digitalWrite(luz, LOW);
    digitalWrite(sala, LOW);
    digitalWrite(patio, LOW);
    digitalWrite(cocina, LOW);
    digitalWrite(todas, LOW);
    digitalWrite(desbloquearAire, LOW);
    digitalWrite(desbloquearLuces, LOW);
// *****************
// Lo Nuevo
     
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");

    // summary of inferencing settings (from model_metadata.h)
    
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) /
                                            sizeof(ei_classifier_inferencing_categories[0]));

    
    run_classifier_init();
    if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false) {
        ei_printf("ERR: Failed to setup audio sampling\r\n");
        return;
    }
   
   
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */

 
// *****************
// Lo Nuevo
void turn_off_leds(){
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    
}
// *****************
// Lo Nuevo
/*
 * Idle:      [0] ==> All OFF
 * lift:       [1] ==> Green ON
 * maritime:   [2] ==> Red ON
 * terrestrial:[3] ==> Blue ON 
 * Anomaly         ==> LED_BUILTIN ON
 */

// *****************
// Lo Nuevo
void comandos_de_voz(int pred_index) {
  switch (pred_index)
  {
    case 0:     // Idle:      [0] ==> All OFF
      turn_off_leds();
      if(Comando == 1){
        if(encenderAire == 1){
         apagarAire = 0;
        }
        if(apagarAire== 0 && seleccionLuz== 0){
          digitalWrite(LEDB, LOW);
          Serial.print("\n..Recibiendo Orden de aire acondicionado..\n");
          apagarAire = 1;
          Comando = 0;
          Detener = 1;
          digitalWrite(aire, HIGH);
          delay(300);
          digitalWrite(aire, LOW);
          delay(5000);
          Serial.print("\n..Continuando muestreo de microfono..\n");
          Detener = 0;
        }
      }
      break;

    case 1:     // lift:       [1] ==> Green ON
      turn_off_leds();         
      if(Comando == 1){
        if(encenderLuces == 1){
          apagarCocina = 0;
        }
        if(apagarCocina==0 && seleccionLuz==1){
          digitalWrite(LEDR, LOW);
          Serial.print("\n..Recibiendo Orden, encendiendo luz en cocina..\n");
          apagarCocina = 1;
          Comando = 0;
          Detener = 1;
          seleccionLuz = 0;
          digitalWrite(cocina, HIGH);
          delay(300);
          digitalWrite(cocina, LOW);
          delay(5000);
          Serial.print("\n..Continuando muestreo de microfono..\n");
          Detener = 0;
        }
      }
      break;
    
    case 2:     // maritime:   [2] ==> Red ON
      turn_off_leds();
      digitalWrite(LEDR, LOW);
      Serial.print("\n..Activando modo de Comandos..\n");
      Detener = 1;
      if(Comando == 0){
       Comando = 1;
       digitalWrite(orden, HIGH);
       delay(300);
       digitalWrite(orden, LOW);
      }
      delay(5000);
      Serial.print("\n..Continuando muestreo de microfono..\n");
      Detener = 0;
      break;

    case 3:      //terrestrial:[3] ==> Blue ON
      turn_off_leds();
      
      if(Comando == 1){
        if(apagarCocina==0 || apagarPatio==0 || apagarSala==0){
          digitalWrite(LEDR, LOW);
          Serial.print("\n..Recibiendo Orden de Luz..\n");
          Detener = 1;
          seleccionLuz = 1;
          digitalWrite(luz, HIGH);
          delay(300);
          digitalWrite(luz, LOW);
          delay(5000);
          Serial.print("\n..Continuando muestreo de microfono..\n");
          Detener = 0;
        }
      }
      break;

    case 4:
      turn_off_leds();
      
      if(Comando == 1){
        if(encenderLuces == 1){
          apagarPatio = 0;
          }
          if(apagarPatio==0 && seleccionLuz==1){
            apagarPatio = 1;
            Comando = 0;
            digitalWrite(LEDR, LOW);
            Serial.print("\n..Recibiendo Orden, encendiendo luz en patio..\n");
            seleccionLuz = 0;
            Detener = 1;
            digitalWrite(patio, HIGH);
            delay(300);
            digitalWrite(patio, LOW);
            delay(5000);
            Serial.print("\n..Continuando muestreo de microfono..\n");
            Detener = 0;
        }
      }
      break;

    case 5:
      turn_off_leds();
      
      if(Comando == 1){
        if(encenderLuces == 1){
          apagarSala = 0;
        }
        if(apagarSala==0 && seleccionLuz==1){
          apagarSala = 1;
          Comando = 0;
          digitalWrite(LEDR, LOW);
          Serial.print("\n..Recibiendo Orden,encendiendo luz en sala..\n");
          seleccionLuz = 0;
          Detener = 1;
          digitalWrite(sala, HIGH);
          delay(300);
          digitalWrite(sala, LOW);
          delay(5000);
          Serial.print("\n..Continuando muestreo de microfono..\n");
          Detener = 0;
        }
      }
      break;
      
    case 6:
      turn_off_leds();
 
      if(Comando == 1){
        if(seleccionLuz==1){ 
          digitalWrite(LEDR, LOW);
          Serial.print("\n..Recibiendo Orden, encendiendo todas las luces..\n");
          Detener = 1;
          seleccionLuz = 0;
          Comando = 0;
          digitalWrite(todas, HIGH);
          delay(300);
          digitalWrite(todas, LOW);
          delay(5000);
          digitalWrite(todas, LOW);
          Serial.print("\n..Continuando muestreo de microfono..\n");
          Detener = 0;
        }
      }
      break;

    case 7:
      turn_off_leds();
      digitalWrite(LEDG, LOW);
      break;
  }
}
// *****************
// Lo Nuevo

void micro (){
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }
 // *****************
// Lo Nuevo 
    int pred_index = 0;
    float pred_value = result.classification[0].value;   
 // *****************
// Lo Nuevo

    if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)) {
        // print the predictions
        ei_printf("Predictions ");
        ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
            result.timing.dsp, result.timing.classification, result.timing.anomaly);
        ei_printf(": \n");
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            ei_printf("    %s: %.5f\n", result.classification[ix].label,
                      result.classification[ix].value);
 // *****************
// Lo Nuevo  
        if (result.classification[ix].value > pred_value){
          pred_index = ix;
          pred_value = result.classification[ix].value;
          
        }  
 // *****************
// Lo Nuevo              
        }
// Lo Nuevo
        comandos_de_voz (pred_index);
 // *****************
// Lo Nuevo

        
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

        print_results = 0;
    } 

}

void loop()
{
    contadorLuces = digitalRead(desbloquearLuces);
    contadorAire = digitalRead(desbloquearAire);
    bool m = microphone_inference_record();
    if (!m) {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }
    if(Detener == 0){
      micro();
    } 

    if(contadorAire == HIGH){
       encenderAire = 1;
      }
    if(contadorLuces == HIGH){
      encenderLuces = encenderLuces + 1;
      if(encenderLuces >= 4){
        encenderLuces = 0;
        }     
      }
}

/**
 * @brief      PDM buffer full callback
 *             Get data and call audio thread callback
 */
static void pdm_data_ready_inference_callback(void)
{
    int bytesAvailable = PDM.available();

    // read into the sample buffer
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

    if (record_ready == true) {
        for (int i = 0; i<bytesRead>> 1; i++) {
            inference.buffers[inference.buf_select][inference.buf_count++] = sampleBuffer[i];

            if (inference.buf_count >= inference.n_samples) {
                inference.buf_select ^= 1;
                inference.buf_count = 0;
                inference.buf_ready = 1;
            }
        }
    }
}

/**
 * @brief      Init inferencing struct and setup/start PDM
 *
 * @param[in]  n_samples  The n samples
 *
 * @return     { description_of_the_return_value }
 */
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffers[0] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[0] == NULL) {
        return false;
    }

    inference.buffers[1] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[1] == NULL) {
        free(inference.buffers[0]);
        return false;
    }

    sampleBuffer = (signed short *)malloc((n_samples >> 1) * sizeof(signed short));

    if (sampleBuffer == NULL) {
        free(inference.buffers[0]);
        free(inference.buffers[1]);
        return false;
    }

    inference.buf_select = 0;
    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;

    // configure the data receive callback
    PDM.onReceive(&pdm_data_ready_inference_callback);

    PDM.setBufferSize((n_samples >> 1) * sizeof(int16_t));

    // initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY)) {
        ei_printf("Failed to start PDM!");
    }

    // set the gain, defaults to 20
    PDM.setGain(127);

    record_ready = true;

    return true;
}

/**
 * @brief      Wait on new data
 *
 * @return     True when finished
 */
static bool microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 10) {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0) {
        delay(100);
    }

    inference.buf_ready = 0;

    return ret;
}

/**
 * Get raw audio signal data
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffers[inference.buf_select ^ 1][offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
static void microphone_inference_end(void)
{
    PDM.end();
    free(inference.buffers[0]);
    free(inference.buffers[1]);
    free(sampleBuffer);
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif
