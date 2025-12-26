/**
 * @file drv8305_app.h
 * @brief DRV8305 Application Layer - Hardware Integration Interface
 * @details Declares application-level wrapper functions and hardware callbacks
 *          for the DRV8305 motor gate driver on TI C2000 DSP platform.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This header provides the application layer interface:
 *   - Public wrapper functions (initialize, polling, timer, motor control)
 *   - Hardware-specific GPIO and SPI callback declarations
 *   - Status and control register callback adapters
 * 
 * @hardware_platform
 * Texas Instruments C2000 DSP (TMS320F28335 or compatible)
 * - GPIO: EN_GATE (gate driver enable), DRV_WAKE (sleep mode control)
 * - SPI:  SPIA (16-bit blocking SPI communication with DRV8305)
 */

#ifndef DRV8305_UTILS_DRV8305_APP_H_
#define DRV8305_UTILS_DRV8305_APP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

#include "drv8305_macros.h"

/**
 * @brief Initialize DRV8305 driver and hardware callbacks
 * @details Initializes the DRV8305 driver instance and sets up all required
 *          hardware callbacks (GPIO, SPI) for the gate driver operation.
 * @return None
 * @note Wrapper for drv8305_api_initialize() with global user object
 * @see drv8305_api_initialize(), drv8305_timer(), drv8305_polling()
 */
DRV8305_PUBLIC void drv8305_initialize(void);

/**
 * @brief Execute timer tick for driver state machine
 * @details Increments the driver's internal timer counter for state machine timing.
 *          Should be called periodically (e.g., 1ms) from a timer interrupt.
 * @return None
 * @note Wrapper for drv8305_api_timer() with global user object
 * @see drv8305_api_timer(), drv8305_polling()
 */
DRV8305_PUBLIC void drv8305_timer(void);

/**
 * @brief Execute main state machine polling cycle
 * @details Runs one iteration of the three-tier hierarchical state machine.
 *          Manages initialization, register polling, configuration, and motor control states.
 * @return None
 * @note Wrapper for drv8305_api_master_sm_polling() with global user object
 * @see drv8305_api_master_sm_polling(), drv8305_initialize(), drv8305_timer()
 */
DRV8305_PUBLIC void drv8305_polling(void);

/**
 * @brief Stop motor (disable DRV8305 gate drivers)
 * @details Application-level convenience function to disable gate drivers.
 *          Calls hardware disable callback to stop motor operation.
 * @return None
 * @see drv8305_ic_enable, drv8305_api_ic_disable
 */
DRV8305_PUBLIC void drv8305_ic_disable(void);

/**
 * @brief Start motor (enable DRV8305 gate drivers)
 * @details Application-level convenience function to enable gate drivers after
 *          driver initialization and configuration is complete.
 * @return None
 * @see drv8305_ic_disable, drv8305_api_ic_enable
 */
DRV8305_PUBLIC void drv8305_ic_enable(void);

/**
 * @brief Confirm DRV8305 configuration completion
 * @details Sends confirmation of register configuration to the DRV8305 IC.
 *          This function should be called after all control registers have been
 *          programmed to acknowledge configuration completion.
 * @return None
 * @note Wrapper for drv8305_api_confirm_configuration() with global user object
 * @see drv8305_api_confirm_configuration(), drv8305_initialize()
 */
DRV8305_PUBLIC void drv8305_confirm_configuration(void);

/**
 * @brief Check if DRV8305 configuration is confirmed
 * @details Calls the core API function to check configuration confirmation status.
 * @return bool True if configuration is confirmed, false otherwise
 * @note Wrapper function using global user_drv8305_obj instance
 * @see drv8305_api_is_configuration_confirm(), drv8305_initialize()
 */
DRV8305_PUBLIC bool drv8305_is_configuration_confirm(void);

/**
 * @brief Reset DRV8305 driver and hardware I/O
 * @details Disables DRV8305 gate drivers and sleep mode, then reinitializes
 *          the driver state machine and configuration.
 */
DRV8305_PUBLIC void drv8305_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_UTILS_DRV8305_APP_H_ */
