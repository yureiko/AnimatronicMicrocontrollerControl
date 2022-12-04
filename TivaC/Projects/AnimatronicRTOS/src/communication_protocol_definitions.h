/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/**
 * File: communication_protocol_definitions.h
 * Brief: Defines all communication messages IDs
 * Author: Yuri Andreiko
 * Year: 2022
*/

#ifndef COMMUNICATION_PROTOCOL_DEFINITIONS_H
#define COMMUNICATION_PROTOCOL_DEFINITIONS_H

#define ID_SET_SERVO_POSITION                      0x00
#define ID_SET_DEFAULT_SERVO_POSITION              0x01
#define ID_RESET_SERVO_POSITION                    0x02
#define ID_RESET_ALL_SERVOS_TO_DEFAULT_POSITION    0x03
#define ID_MOVE_EYES                               0x04
#define ID_MOVE_EYELIDS                            0x05
#define ID_MOVE_EYEBROWS                           0x06
#define ID_MOVE_MOUTH                              0x07
#define ID_MOVE_EARS                               0x08

#endif