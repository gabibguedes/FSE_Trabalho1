#include "modbus.h"
#include "uart.h"
#include "crc.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_modbus_message (unsigned char *message, int message_size) {
  int pos = 0;
  short crc;
  unsigned char *message_buffer;

  message_buffer = malloc(MIN_MODBUS_SIZE + message_size);

  message_buffer[pos++] = TO_DEVICE_CODE;

  if (message_size > 1)
    message_buffer[pos++] = SEND_CODE_MODBUS;
  else
    message_buffer[pos++] = REQUEST_CODE_MODBUS;

  for (int i = 0; i < message_size; i++)
    message_buffer[pos++] = message[i];

  crc = calcula_CRC(message_buffer, pos);
  memcpy(&message_buffer[pos], &crc, CRC_SIZE);
  pos += CRC_SIZE;

  write_in_uart(message_buffer, pos);
}

int modbus_error(unsigned char *buffer){
  int error = 0;
  if (buffer[0] != THIS_DEVICE_CODE) {
    printf("[ERRO] Device errado! %x\n", buffer[0]);
    error++;
  }

  if (buffer[1] != SEND_CODE_MODBUS && buffer[1] != REQUEST_CODE_MODBUS) {
    printf("[ERRO] Código MODBUS errado! %x\n", buffer[1]);
    error++;
  }

  return error;
}

unsigned char *receive_modbus_message() {
  int buffer_size;
  unsigned char *buffer, *message;
  // short crc;

  buffer = read_uart();
  buffer_size = strlen((char *)&buffer[1]) + 1;

  message = malloc(buffer_size);

  if (modbus_error(buffer))
    return NULL;

  // TODO: verify CRC
  // crc = calcula_CRC(buffer, size - CRC_SIZE);
  // memcpy(crc_buffer, &crc, CRC_SIZE);

  memcpy(message, &buffer[2], buffer_size);

  return message;
}
