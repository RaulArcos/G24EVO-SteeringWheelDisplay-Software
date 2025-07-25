/**
 * @file data_processor.cpp
 * @author Raúl Arcos Herrera
 * @brief This file contains the implementation of the Data Processor class for Link G4+ ECU.
 */

#include "../include/data_processor.hpp"

void DataProcessor::send_serial(byte type, unsigned int value) {                     //Como parámetros se pasan el ID (type), que es el ID establecido al inicio del código para el dato que se quiera enviar. Ej: RPM_ID -> 0x51; y se envía el valor de dicho dato.
    byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };  //Se establece un arreglo de bytes con los primeros datos necesarios para que la pantalla lo interprete como mensaje (En la Wiki hay tutoriales que lo explican a fondo), como ser la longitud y el tipo de mensaje.
    dato[4] = type;                                                     //Se configura en el mensaje el ID correspondiente al dato a enviar.
    dato[6] = (value >> 8) & 0xFF;                                      //Se configura el dato en los últimos 2 bytes.
    dato[7] = value & 0xFF;

    Serial.write(dato, 8);                                              //Se envía serialmente el mensaje, indicando su longituden bytes para ello.
}

//RPM + TPS + vBatt + ECT
void DataProcessor::send_serial_frame_0(int rpmh, int rpml, int tpsh, int tpsl, int vbatth, int vbattl, int ect){
    Serial.println("send_serial_frame_0");
    
    int rpm = (rpmh * 256) + rpml; 
    int tps = (tpsh * 256) + tpsl; 
    double vbatt = ((vbatth * 256) + vbattl) / 100.0; 

    _crow_panel_controller->set_value_to_label(ui_rpm, rpm);
    _crow_panel_controller->set_value_to_label(ui_battvolt, vbatt);
    _crow_panel_controller->set_value_to_label(ui_ect, ect);
    _crow_panel_controller->set_value_to_label(ui_ect2, ect);
}

//LAMB + LAMBTRG + FUEL + GEAR
void DataProcessor::send_serial_frame_1(int lmbh, int lmbl, int lmbth, int lmbtl, int fuelh, int fuell, int gear){
    Serial.println("send_serial_frame_1");
   int lmb = (lmbh * 256) + lmbl;
   int lmbtrg = (lmbth * 256) + lmbtl;
   int fuel = (fuelh * 256) + fuell;
   _crow_panel_controller->set_value_to_label(ui_lambda, lmb);
   _crow_panel_controller->set_value_to_label(ui_lambdatarget, lmbtrg);
   _crow_panel_controller->set_value_to_label(ui_fuel, fuel);
   _crow_panel_controller->set_value_to_label(ui_gear, gear);
}


void DataProcessor::send_serial_frame_2(int shut, int fan, int lmbch, int lmbcl, int brakeh, int brakel, int aux1){
    Serial.println("send_serial_frame_2");
    int lmbcorrect = (lmbch * 256) + lmbcl;
    int brake = (brakeh * 256) + brakel;

    _crow_panel_controller->set_value_to_label(ui_shutdown, shut);
    _crow_panel_controller->set_value_to_label(ui_fan, fan);
    _crow_panel_controller->set_value_to_label(ui_correctionlambda, lmbcorrect);
    _crow_panel_controller->set_value_to_label(ui_auxstatus9, brake);
    _crow_panel_controller->set_value_to_label(ui_auxstatus1, aux1);
}

void DataProcessor::send_serial_frame_3(int aux3, int aux4, int aux5, int aux6, int aux7, int aux8, int dig1){
    Serial.println("send_serial_frame_3");

    _crow_panel_controller -> set_value_to_label(ui_auxstatus3, aux3);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus4, aux4);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus5, aux5);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus6, aux6);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus7, aux7);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus8, aux8);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus1, dig1);    
}

void DataProcessor::send_serial_frame_4(int dig3, int dig4, int dig5, int dig6, int dig7, int dig8, int dig9){
    Serial.println("send_serial_frame_4");

    _crow_panel_controller -> set_value_to_label(ui_digitalstatus3, dig3);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus4, dig4);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus5, dig5);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus6, dig6);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus7, dig7);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus8, dig8);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus9, dig9);
}

void DataProcessor::send_serial_screen_test(int test) {
    _crow_panel_controller->set_value_to_label(ui_rpm, test);
    Serial.println(test);
}