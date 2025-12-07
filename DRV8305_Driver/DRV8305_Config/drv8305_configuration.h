/**
 * @file drv8305_configuration.h
 * @brief DRV8305 Configuration Module - Register Settings Management
 * @details Defines configuration structure and default settings for all DRV8305
 *          control registers (gate drive, IC operation, sensing, regulation).
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module manages DRV8305 configuration:
 *   - Defines drv8305_configuration_t structure aggregating all 7 control registers
 *   - Provides getter/setter functions for configuration access
 *   - Maintains default configuration from DRV8305-Q1 datasheet reset values
 *   - Supports configuration read and write during driver operation
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
#include "drv8305_control_registers_definitions.h"

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

// ============================================================================
// PUBLIC CONFIGURATION INTERFACE - Get/Set Functions
// ============================================================================

/**
 * @brief Get current DRV8305 configuration
 * @details Retrieves pointer to the active default_configuration structure
 *          containing all current control register settings (gate drive,
 *          IC operation, shunt amplifier, voltage regulator, VDS sensing).
 * @return drv8305_configuration_t* Non-NULL pointer to default configuration structure
 * @note Pointer remains valid throughout driver lifetime
 * @usage Modify configuration settings and apply with drv8305_set_configuration()
 * @see drv8305_set_configuration(), drv8305_configuration_t
 * 
 * @example
 * @code
 * // Get current configuration
 * drv8305_configuration_t* config = drv8305_get_configuration();
 * 
 * // Modify gate drive settings
 * config->hs_gate_drive.peak_current = DRV8305_CTRL05_IPEAK_2_75A;
 * config->gate_drive.pwm_mode = DRV8305_PWM_MODE_PWM_SYNC;
 * 
 * // Apply new configuration to IC
 * drv8305_set_configuration(config);
 * @endcode
 */
DRV8305_PUBLIC drv8305_configuration_t* drv8305_get_configuration(void);

/**
 * @brief Set new DRV8305 configuration
 * @details Copies provided configuration structure into the internal
 *          default_configuration, replacing all control register settings.
 *          Changes take effect after drv8305_api_confirm_configuration() call.
 * @param[in] cfg Pointer to new configuration structure to apply
 * @return None
 * @note Entire configuration is copied (memcpy operation)
 * @warning Configuration changes require drv8305_api_confirm_configuration() to take effect
 * @see drv8305_get_configuration(), drv8305_api_confirm_configuration()
 * 
 * @example
 * @code
 * // Create new configuration
 * drv8305_configuration_t new_config = {
 *     .hs_gate_drive = { .peak_current = DRV8305_CTRL05_IPEAK_1_70A },
 *     .ls_gate_drive = { .peak_current = DRV8305_CTRL06_IPEAK_1_70A },
 *     // ... other settings
 * };
 * 
 * // Apply new configuration
 * drv8305_set_configuration(&new_config);
 * drv8305_api_confirm_configuration();  // Must call to apply changes to IC
 * @endcode
 */
DRV8305_PUBLIC void drv8305_set_configuration(drv8305_configuration_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_CONFIGURATION_H_ */
