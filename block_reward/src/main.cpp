#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "dummy_params.h"
#include "GetBlockSubsidy.snip"

int main()
{
	Consensus::Params defaults;
	CAmount total_reward = 0;
	bool over_cap = false;
	int block_height;

	printf("block_height, reward, total_reward\n");

	for(block_height = 1; block_height < 42602000; block_height++)
	{
		static CAmount last_reward = 0;
		CAmount reward = GetBlockSubsidy(block_height, defaults);
		total_reward += reward;
		if(!over_cap)
		{
			if(total_reward >= (21000000000 * COIN))
			{
				over_cap = true;
				time_t final_block_time = (block_height - 13168400) * 15 + 1624402422;	// calibrated off of block 13168400 with 15 second block times
				struct tm *my_tm = gmtime(&final_block_time);
				char text[48];
				strftime(text, sizeof(text), "%F", my_tm);
				printf("%d,%ld.%08ld,%ld.%08ld, on, %s\n", block_height, reward / COIN, reward % COIN, total_reward / COIN, total_reward % COIN, text);
			}
		}
		if(reward != last_reward)
		{
			printf("%d,%ld.%08ld,%ld.%08ld\n", block_height, reward / COIN, reward % COIN, total_reward / COIN, total_reward % COIN);
			last_reward = reward;
		}
	}
}

