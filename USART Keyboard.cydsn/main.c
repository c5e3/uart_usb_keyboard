#include <stdio.h>
#include <project.h>
#include "keyboard_scancodes.h"

#define KEYBOARD_ENDPOINT 0x01u
#define KEYBOARD_DEVICE 0x00u
#define KEYBOARD_DATA_SIZE 0x08u
#define KEY_DATA_INDEX 0x02u
#define MODIFIER_KEY_DATA_INDEX 0x00u

static void SendKey(uint8 key);
static void StartWindowsRun(void);
static void GetAckLoadEp(uint8 * keyboardData);

int main(void){
    uint8 UsartRec;
    
    uint8 ledState = 0;

    uint8 keyboardData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    char ascii[] = { key_nul, key_soh, key_stx, key_etx, key_eot, key_enq, key_ack, key_bel, key_bs, key_ht, key_lf, key_vt, key_ff, key_cr, key_so, key_si, key_dle, key_dc1, key_dc2, key_dc3, key_dc4, key_nak, key_syn, key_etb, key_can, key_em, key_sub, key_esc, key_fs, key_gs, key_rs, key_us, key_space, key_exclam, key_quote, key_sharp, key_dollar, key_percent, key_and, key_tick, key_bropn, key_brcls, key_asterisk, key_plus, key_comma, key_minus, key_dot, key_slash, key_0, key_1, key_2, key_3, key_4, key_5, key_6, key_7, key_8, key_9, key_colon, key_semicolon, key_smaller, key_equal, key_larger, key_quest, key_at, key_a, key_c, key_b, key_d, key_e, key_f, key_g, key_h, key_i, key_j, key_k, key_l, key_m, key_n, key_o, key_p, key_q, key_r, key_s, key_t, key_u, key_v, key_w, key_x, key_z, key_y, key_sqbropn, key_backslash, key_sqbrcls, key_circumflex, key_underline, key_gravis, key_a, key_c, key_b, key_d, key_e, key_f, key_g, key_h, key_i, key_j, key_k, key_l, key_m, key_n, key_o, key_p, key_q, key_r, key_s, key_t, key_u, key_v, key_w, key_x, key_z, key_y, key_curlbropn, key_pipe, key_curlbrcls, key_tilde, key_del };
    
    UART_1_Start();
    UART_1_PutString("USART to USB Keyboard\r\n");
    
    CYGlobalIntEnable;
    USBFS_1_Start(KEYBOARD_DEVICE, USBFS_1_5V_OPERATION);
    while(!USBFS_1_bGetConfiguration());
    USBFS_1_LoadInEP(KEYBOARD_ENDPOINT, keyboardData, KEYBOARD_DATA_SIZE);
    
    while(1){
        UsartRec = UART_1_GetChar();
        if(UsartRec){
            if(UsartRec != '\r' && UsartRec != 8){
                SendKey(ascii[UsartRec]);
                UART_1_PutChar(UsartRec);
            } else if(UsartRec == '\r'){
                SendKey(key_enter);
                UART_1_PutString("\r\n");
            } else if(UsartRec == 8){
                SendKey(key_backsp);
                UART_1_PutChar(UsartRec);
            }
            ledState ^= 0x01u;
            LED_Write(ledState);
        }
    }
}

static void SendKey(uint8 key){
    uint8 keyboardData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    keyboardData[KEY_DATA_INDEX] = key;
    GetAckLoadEp(keyboardData);

    keyboardData[KEY_DATA_INDEX] = key_release;
    GetAckLoadEp(keyboardData);
}

static void GetAckLoadEp(uint8 * keyboardData){
    while(!USBFS_1_bGetEPAckState(KEYBOARD_ENDPOINT));
    USBFS_1_LoadInEP(KEYBOARD_ENDPOINT, keyboardData, KEYBOARD_DATA_SIZE);
}
