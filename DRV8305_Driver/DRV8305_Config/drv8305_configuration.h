/**
 * @file drv8305_configuration.h
 * @brief DRV8305 Configuration Module - Register Settings Management
 * @details Defines configuration structure for all DRV8305 control registers
 *          (gate drive, IC operation, sensing, regulation). Function declarations
 *          have been moved to drv8305_api.h for unified API access.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module manages DRV8305 configuration structure:
 *   - Defines drv8305_configuration_t structure aggregating all 7 control registers
 *   - Provides default configuration from DRV8305-Q1 datasheet reset values
 *   - Note: drv8305_get_configuration() and drv8305_set_configuration() declared in drv8305_api.h
 * 
 * @configuration_includes
 *   - HS/LS Gate Drive Control (peak current drive time, sink/source currents)
 *   - Gate Drive Control (charge pump freq, commutation, PWM mode, dead time)
 *   - IC Operation (watchdog, undervoltage, fault settings)
 *   - Shunt Amplifier Control (DC calibration, gain for 3 current channels)
 *   - Voltage Regulator Control (VREF scaling, sleep delay, UV thresholds)
 *   - VDS Sense Control (threshold level, response mode)
 */

#ifndef DRV8305_CONFIGURATION_H_
#define DRV8305_CONFIGURATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "drv8305_macros.h"
#include "DRV8305_Control_Registers/drv8305_control_registers_definitions.h"

/**
 * @brief Complete DRV8305 configuration structure
 * Contains all control register configurations
 */
typedef struct
{
    drv8305_ctrl05_hs_gate_t hs_gate_drive;
    drv8305_ctrl06_ls_gate_t ls_gate_drive;
    drv8305_ctrl07_gate_t    gate_drive;
    drv8305_ctrl09_ic_op_t   ic_operation;
    drv8305_ctrl0A_shunt_t   shunt_amplifier;
    drv8305_ctrl0B_vreg_t    voltage_regulator;
    drv8305_ctrl0C_vds_t     vds_sense;
} drv8305_configuration_t;

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_CONFIGURATION_H_ */
