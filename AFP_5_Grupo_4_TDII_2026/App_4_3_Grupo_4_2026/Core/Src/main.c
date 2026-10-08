/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Archivo principal del programa
  **************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "API_GPIO.h"
#include "API_Delay.h"
#include "API_Debounce.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Nombres descriptivos para los pines de los LEDs */
#define LED_VERDE LD1_Pin
#define LED_AZUL  LD2_Pin
#define LED_ROJO  LD3_Pin

/* Tiempos de duracion para cada secuencia (en milisegundos) */
#define TIEMPO_SEC_0 150
#define TIEMPO_SEC_1 300
#define TIEMPO_SEC_2 100
#define TIEMPO_SEC_3 150

/* Cantidad maxima de secuencias (de 0 a 3, son 4 secuencias en total) */
#define MAX_SECUENCIAS 3

/* Estado que indica boton presionado */
#define BOTON_ACTIVO 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT];
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT];

ETH_HandleTypeDef heth;
UART_HandleTypeDef huart3;
PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
/* Cronometro para manejar el ritmo de las luces */
delay_t delay_secuencia;

/* Variable que controla el momento actual de la secuencia de luces */
int paso = 0;

/* Memoria del estado del boton para detectar el momento exacto del toque */
bool_t estado_boton_anterior = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* Variable que indica que secuencia se esta ejecutando actualmente */
  uint8_t secuencia = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();

  /* USER CODE BEGIN 2 */
  /* Se inicializa el cronometro con el tiempo de la primera secuencia */
  delayInit(&delay_secuencia, TIEMPO_SEC_0);

  /* Se prepara el sistema para leer el boton y evitar falsos contactos */
  debounceFSM_init();
  /* USER CODE END 2 */

  /* Bucle infinito del programa */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* Actualizacion del lector del boton */
      /* Se envia la lectura fisica del pin al sistema anti-rebote */
      debounceFSM_update(readButton_GPIO() == BOTON_ACTIVO);

      /* Se consulta si el sistema confirmo una pulsacion valida */
      bool_t estado_boton_actual = readKey();

      /* Deteccion del momento exacto de pulsacion (Flanco) */
      /* Verifica si el boton acaba de ser presionado y antes no lo estaba */
      if (estado_boton_actual == true && estado_boton_anterior == false)
      {
          secuencia++;
          if (secuencia > MAX_SECUENCIAS)
          {
              secuencia = 0;
          }

          /* Se apagan todas las luces para evitar interferencias al cambiar de modo */
          writeLedOff_GPIO(LED_VERDE);
          writeLedOff_GPIO(LED_AZUL);
          writeLedOff_GPIO(LED_ROJO);

          paso = 0;
      }

      /* Se guarda el estado actual para la comparacion del proximo ciclo */
      estado_boton_anterior = estado_boton_actual;


      /* Ejecucion de las secuencias de luces */
      /* Solo avanza si el cronometro indica que ya paso el tiempo necesario */
      if(delayRead(&delay_secuencia) == true) {

          /* SECUENCIA 0 */
          if (secuencia == 0) {
              delayWrite(&delay_secuencia, TIEMPO_SEC_0);

              if(paso == 0)      { writeLedOn_GPIO(LED_VERDE); }
              else if(paso == 1) { writeLedOff_GPIO(LED_VERDE); }
              else if(paso == 2) { writeLedOn_GPIO(LED_AZUL); }
              else if(paso == 3) { writeLedOff_GPIO(LED_AZUL); }
              else if(paso == 4) { writeLedOn_GPIO(LED_ROJO); }
              else if(paso == 5) { writeLedOff_GPIO(LED_ROJO); paso = -1; }
              paso++;
          }

          /* SECUENCIA 1 */
          else if (secuencia == 1) {
              delayWrite(&delay_secuencia, TIEMPO_SEC_1);

              if(paso == 0) {
                  writeLedOn_GPIO(LED_VERDE); writeLedOn_GPIO(LED_AZUL); writeLedOn_GPIO(LED_ROJO);
              }
              else if(paso == 1) {
                  writeLedOff_GPIO(LED_VERDE); writeLedOff_GPIO(LED_AZUL); writeLedOff_GPIO(LED_ROJO);
                  paso = -1;
              }
              paso++;
          }

          /* SECUENCIA 2 */
          else if (secuencia == 2) {
              delayWrite(&delay_secuencia, TIEMPO_SEC_2);

              if(paso == 0)      { toggleLed_GPIO(LED_VERDE); toggleLed_GPIO(LED_AZUL); toggleLed_GPIO(LED_ROJO); }
              else if(paso == 1) { toggleLed_GPIO(LED_VERDE); }
              else if(paso == 2) { toggleLed_GPIO(LED_VERDE); }
              else if(paso == 3) { toggleLed_GPIO(LED_VERDE); toggleLed_GPIO(LED_AZUL); }
              else if(paso == 4) { toggleLed_GPIO(LED_VERDE); }
              else if(paso == 5) { toggleLed_GPIO(LED_VERDE); paso = -1; }
              paso++;
          }

          /* SECUENCIA 3 */
          else if (secuencia == 3) {
              delayWrite(&delay_secuencia, TIEMPO_SEC_3);

              if(paso == 0) {
                  writeLedOn_GPIO(LED_VERDE); writeLedOn_GPIO(LED_ROJO); writeLedOff_GPIO(LED_AZUL);
              }
              else if(paso == 1) {
                  writeLedOff_GPIO(LED_VERDE); writeLedOff_GPIO(LED_ROJO); writeLedOn_GPIO(LED_AZUL);
                  paso = -1;
              }
              paso++;
          }
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
}
/* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{
   static uint8_t MACAddr[6];
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
