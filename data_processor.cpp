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
    Serial.println(rpmh);
    Serial.println(rpml);
    Serial.println(tpsh);
    Serial.println(tpsl);
    Serial.println(vbatth);
    Serial.println(vbattl);
    Serial.println(ect);
    
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


void DataProcessor::send_serial_frame_2(bool shut, bool fan, int lmbch, int lmbcl, int brakeh, int brakel, bool aux1, bool aux2){
    Serial.println("send_serial_frame_2");
    int lmbcorrect = (lmbch * 256) + lmbcl;
    int brake = (brakeh * 256) + brakel;

    _crow_panel_controller -> set_value_to_label(ui_shutdown, shut);
    _crow_panel_controller -> set_value_to_label(ui_fan, fan);
    _crow_panel_controller -> set_value_to_label(ui_correctionlambda, lmbcorrect);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus9, brake);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus1, aux1);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus2, aux2);
}

void DataProcessor::send_serial_frame_3(bool aux3, bool aux4, bool aux5, bool aux6, bool aux7, bool aux8, bool dig1, dig2){
    Serial.println("send_serial_frame_3");

    _crow_panel_controller -> set_value_to_label(ui_auxstatus3, aux3);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus4, aux4);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus5, aux5);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus6, aux6);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus7, aux7);
    _crow_panel_controller -> set_value_to_label(ui_auxstatus8, aux8);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus1, dig1);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus1, dig2);
    
}

void DataProcessor::send_serial_frame_4(bool dig3, bool dig4, bool dig5, bool dig6, bool dig7, bool dig8, dig9, dig10){
    Serial.println("send_serial_frame_4");

    _crow_panel_controller -> set_value_to_label(ui_digitalstatus3, dig3);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus4, dig4);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus5, dig5);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus6, dig6);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus7, dig7);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus8, dig8);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus9, dig9);
    _crow_panel_controller -> set_value_to_label(ui_digitalstatus10, dig10);
}

// void DataProcessor::send_serial_frame_2(int lambh, int lambl, int lamth, int lamtl, int bvolth, int bvoltl, int iat){
//     byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };
//     dato[4] = LAMBDA_ID;
//     dato[6] = lambh & 0xFF;
//     dato[7] = lambl & 0xFF;
//     Serial.write(dato, 8);

//     dato[4] = BVOLT_ID;
//     dato[6] = bvolth & 0xFF;
//     dato[7] = bvoltl & 0xFF;
//     Serial.write(dato, 8);

//     dato[4] = IAT_ID;
//     dato[6] = iat & 0xFF;
//     dato[7] = 0;
//     Serial.write(dato, 8);
// }

// void DataProcessor::send_serial_change_display(int display){
//     byte dato[10] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, 0x00};
//     const int displayIDs[] = { DISPLAY_0_ID, DISPLAY_1_ID, DISPLAY_2_ID, DISPLAY_3_ID, DISPLAY_4_ID };
//     dato[9] = displayIDs[display];
//     Serial.write(dato, 10);
// }

void DataProcessor::send_serial_screen_test(int test) {
    _crow_panel_controller->set_value_to_label(ui_rpm, test);
    Serial.println(test);
}