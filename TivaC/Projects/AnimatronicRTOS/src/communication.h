/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/**
 * File: communication.h
 * Brief: Manages all received messages from UART, validates and sends data to servo message queue 
 * Author: Yuri Andreiko
 * Year: 2022
*/

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

void communication_thread_init();

#endif