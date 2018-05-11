// STM32CubeMX_ClockConfig.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "math.h"

#define Input_Frequecy_MHZ	25
#define MAX_SYSCLK_MHZ	120
#define MAX_HCLK_MHZ	120
#define MAX_PCLK1_MHZ	30
#define MAX_PCLK2_MHZ	60

#define M_MAX	63
#define M_MIN	2

#define N_MAX	432
#define N_MIN	192


#define AHBPrescaler_MAX	9
#define P_MIN				2
#define P_Prescaler_MAX		4
int main()
{
	for (int m = M_MIN; m < M_MAX + 1; m++)
	{
		for (int n = N_MIN; n < N_MAX + 1; n++)
		{
			for (int p = P_MIN; p < pow(2, P_Prescaler_MAX + 1); p *= 2)
			{
				for (int AHB_presc = 1; AHB_presc < pow(2, AHBPrescaler_MAX + 1); AHB_presc *= 2)
				{
					float Freq_M = Input_Frequecy_MHZ / (float)m;
					float Freq_N = Freq_M * (float)n;
					float Freq_P = Freq_N / (float)p;
					float Freq_sysclk = (float)Freq_P;
					float HCLK = Freq_sysclk / (float)AHB_presc;
					if (Freq_M > 2|| Freq_M < 1 || Freq_N > MAX_SYSCLK_MHZ || Freq_P > MAX_SYSCLK_MHZ || Freq_sysclk > MAX_SYSCLK_MHZ || HCLK > MAX_HCLK_MHZ)
					{
						break;
					}
					else
					{
						printf("M= %4d, N= %4d, P= %4d, AHBPrescaler= %4d, Freq_sysclk=%3.6f, HCLK=%3.6f\r\n", m, n, p, AHB_presc, Freq_sysclk, HCLK);
					}
				}
			}
		}
	}
	getchar();
    return 0;
}

