#include "gpio_control.h"


void gpio_init(void)
{   

    // Initialize Control Pins
    nrf_gpio_cfg_output(PULSE_PIN_1);
    nrf_gpio_cfg_output(PULSE_PIN_2);
    nrf_gpio_cfg_output(PULSE_PIN_3);
    nrf_gpio_cfg_output(PULSE_PIN_4);
    nrf_gpio_cfg_output(PULSE_PIN_5);
    nrf_gpio_cfg_output(PULSE_PIN_6); 
    nrf_gpio_cfg_output(PULSE_PIN_7); 
    nrf_gpio_cfg_output(PULSE_PIN_8); 
    nrf_gpio_cfg_output(PULSE_PIN_9); 
    nrf_gpio_cfg_output(PULSE_PIN_10); 
    nrf_gpio_cfg_output(PULSE_PIN_11); 
    nrf_gpio_cfg_output(PULSE_PIN_12); 

    // Turn off pulsing pins
    nrf_gpio_pin_clear(PULSE_PIN_1);
    nrf_gpio_pin_clear(PULSE_PIN_2);
    nrf_gpio_pin_clear(PULSE_PIN_3);
    nrf_gpio_pin_clear(PULSE_PIN_4);
    nrf_gpio_pin_clear(PULSE_PIN_5);
    nrf_gpio_pin_clear(PULSE_PIN_6);
    nrf_gpio_pin_clear(PULSE_PIN_7);
    nrf_gpio_pin_clear(PULSE_PIN_8);
    nrf_gpio_pin_clear(PULSE_PIN_9);
    nrf_gpio_pin_clear(PULSE_PIN_10);
    nrf_gpio_pin_clear(PULSE_PIN_11);
    nrf_gpio_pin_clear(PULSE_PIN_12);

    uint32_t err_code = app_timer_start(m_app_timer_id, CASCADE_INTERVAL, NULL); // initialize the timer
}

//TODO Pulsing Implementation
void automatic_pulsing_onoff(int onoff)
{
    
}

/* Turn on and off Control Pins */
void control_pin1_onoff(int onoff)
{
    if(onoff == 1) nrf_gpio_pin_set(CONTROL_PIN_1);
    else nrf_gpio_pin_clear(CONTROL_PIN_1);
}

void control_pin2_onoff(int onoff)
{
    if(onoff == 1) nrf_gpio_pin_set(CONTROL_PIN_2);
    else nrf_gpio_pin_clear(CONTROL_PIN_2);
}

void control_pin3_onoff(int onoff)
{
    if(onoff == 1) nrf_gpio_pin_set(CONTROL_PIN_3);
    else nrf_gpio_pin_clear(CONTROL_PIN_3);
}

void control_pin4_onoff(int onoff)
{
    if(onoff == 1) nrf_gpio_pin_set(CONTROL_PIN_4);
    else nrf_gpio_pin_clear(CONTROL_PIN_4);
}

void control_table(ble_nus_evt_t * p_evt)
{
    int idx = 0;
    char buffer_rx[20] = "";
    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        uint32_t err_code;
        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
        for (uint32_t i = 0; i < p_evt->params.rx_data.length; i++)
        {
            do
            {
                err_code = app_uart_put(p_evt->params.rx_data.p_data[i]);
                
                if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_BUSY))
                {
                    NRF_LOG_ERROR("Failed receiving NUS message. Error 0x%x. ", err_code);
                    APP_ERROR_CHECK(err_code);
                }

            } while (err_code == NRF_ERROR_BUSY);
            if(p_evt->params.rx_data.p_data[i] == '<' || p_evt->params.rx_data.p_data[i] == '>') continue;
            buffer_rx[idx] = p_evt->params.rx_data.p_data[i];
            idx++;
        }
        if (p_evt->params.rx_data.p_data[p_evt->params.rx_data.length - 1] == '\r')
        {
            while (app_uart_put('\n') == NRF_ERROR_BUSY);
        }
        
        printf("\r\nCENTRAL RECEIVED MESSAGE:::%s\r\n", buffer_rx);

        // operate LED
        if(strcmp(buffer_rx, "AUTOMATICPULSEON")==0) automatic_pulsing_onoff(1); 
        else if(strcmp(buffer_rx, "AUTOMATICPULSEOFF")==0) automatic_pulsing_onoff(0); 
        else if(strcmp(buffer_rx, "LEDCASCADEON")==0)   led_cascade_on();
        else if(strcmp(buffer_rx, "LEDCASCADEOFF")==0)  led_cascade_off();
        else if(strcmp(buffer_rx, "CONTROLPIN1ON")==0)  control_pin1_onoff(1);
        else if(strcmp(buffer_rx, "CONTROLPIN1OFF")==0) control_pin1_onoff(0); 
        else if(strcmp(buffer_rx, "CONTROLPIN2ON")==0)  control_pin2_onoff(1);
        else if(strcmp(buffer_rx, "CONTROLPIN2OFF")==0) control_pin2_onoff(0);
        else if(strcmp(buffer_rx, "CONTROLPIN3ON")==0)  control_pin3_onoff(1);
        else if(strcmp(buffer_rx, "CONTROLPIN3OFF")==0) control_pin3_onoff(0);
        else if(strcmp(buffer_rx, "CONTROLPIN4ON")==0)  control_pin4_onoff(1);
        else if(strcmp(buffer_rx, "CONTROLPIN4OFF")==0) control_pin4_onoff(0);
    }
}

static void app_timer_handler(void * p_context)
{
      static bool status1, status2, status3, status4, status5;

      status1 = nrf_gpio_pin_out_read(CASCADE_PIN_1);
      status2 = nrf_gpio_pin_out_read(CASCADE_PIN_2);
      status3 = nrf_gpio_pin_out_read(CASCADE_PIN_3);
      status4 = nrf_gpio_pin_out_read(CASCADE_PIN_4);
      status5 = nrf_gpio_pin_out_read(CASCADE_PIN_5);

      if (status1 && status2 && status3 && status4 && status5)
      {
          // Turn off all LEDs
          nrf_gpio_pin_clear(CASCADE_PIN_1);
          nrf_gpio_pin_clear(CASCADE_PIN_2); 
          nrf_gpio_pin_clear(CASCADE_PIN_3); 
          nrf_gpio_pin_clear(CASCADE_PIN_4); 
          nrf_gpio_pin_clear(CASCADE_PIN_5); 
      }
      else if(!status1) nrf_gpio_pin_set(CASCADE_PIN_1);
      else if(!status2) nrf_gpio_pin_set(CASCADE_PIN_2);
      else if(!status3) nrf_gpio_pin_set(CASCADE_PIN_3);
      else if(!status4) nrf_gpio_pin_set(CASCADE_PIN_4);
      else if(!status5) nrf_gpio_pin_set(CASCADE_PIN_5);
}

void create_timer(void)
{
    ret_code_t err_code;
    err_code = app_timer_create(&m_app_timer_id, APP_TIMER_MODE_REPEATED, app_timer_handler);
    APP_ERROR_CHECK(err_code);    
}

/* LED Control Functions */

void led_cascade_on(void)
{
    printf("FUNC LED CASCADE ON\r\n");
    if (app_timer_cnt_get() == 0)
    {
      create_timer();
      app_timer_start(m_app_timer_id, CASCADE_INTERVAL, NULL);  // initialize the timer
    }
}


/* Turn off all LEDs*/
void led_cascade_off(void)
{
    static uint32_t i;
    printf("FUNC LED CASCADE OFF\r\n");
    if (app_timer_cnt_get() != 0)
    {
        app_timer_stop(m_app_timer_id);
        nrf_gpio_pin_clear(CASCADE_PIN_1);
        nrf_gpio_pin_clear(CASCADE_PIN_2); 
        nrf_gpio_pin_clear(CASCADE_PIN_3);
        nrf_gpio_pin_clear(CASCADE_PIN_4);
        nrf_gpio_pin_clear(CASCADE_PIN_5); 
        i = 0;
    }
}


