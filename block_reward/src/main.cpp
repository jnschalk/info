#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "dummy_params.h"
#include "GetBlockSubsidy.snip"

typedef uint32_t BlockID;

void display_help(void);
void display_block_info(BlockID block_height, CAmount reward, CAmount total_reward, bool indicator);

int main(int argc, char *argv[])
{
	Consensus::Params defaults;
	CAmount total_reward = 0;
	bool over_cap = false;
	BlockID block_height;
	BlockID block_info_height = 0;
	int opt;

	while ((opt = getopt(argc, argv, "hb:")) != -1) {
		switch (opt) {
			case 'b':	// displays data for a specified block #
				block_info_height = strtoul(optarg, NULL, 10);
				break;

			default:
			case 'h':
				display_help();
				exit(0);
		}
	}

	printf("block_height, reward, total_reward\n");
// 	output of digibyte-cli gettxoutsetinfo                                                              
// 	{
// 		"height": 13307241,
// 		"bestblock": "d9d02169bfa210c3785b8641e0e297ab80dca88bba9b4942f58f3cc95c87a159",
// 		"transactions": 3076170,
// 		"txouts": 10470013,
// 		"bogosize": 791114141,
// 		"hash_serialized_2": "ae4f5a33b0b7c8d9ccf548d48e1af539f9bdaa0f13e332f55af19ab1df7f85d6",
// 		"disk_size": 566378280,
// 		"total_amount": 14495186190.33644281
// 	}
// calibrated using block 13307241 because not all blocks specified the maximum block reward
	total_reward = 1449518619033644281;
// start @ 13307242 because the block reward for 13307241 is already included in total_reward
	for(block_height = 13307242; block_height < 42802000; block_height++)
	{
		CAmount reward = GetBlockSubsidy(block_height, defaults);
		total_reward += reward;

		if(block_height == block_info_height)
			display_block_info(block_height, reward, total_reward, true);
		if(!over_cap && (total_reward >= (21000000000 * COIN)))
		{
			over_cap = true;
			display_block_info(block_height, reward, total_reward, true);
		}

		static CAmount last_reward = 0;
		if(reward != last_reward)
		{
			display_block_info(block_height, reward, total_reward, false);
			last_reward = reward;
		}
	}
	return 0;
}

void display_help(void)
{
	printf("block_reward:\n");
	printf(" -b <Block Number>  outputs data for the given block number\n");
	printf(" -h                 help\n");
}

void display_block_info(BlockID block_height, CAmount reward, CAmount total_reward, bool indicator)
{
	time_t final_block_time = (block_height - 13168400) * 15 + 1624402422;	// calibrated off of block 13168400 with 15 second block times
	char text[48];
	struct tm *my_tm = gmtime(&final_block_time);
	strftime(text, sizeof(text), "%F", my_tm);

	printf("%d,%ld.%08ld,%ld.%08ld,%s%s\n", block_height, reward / COIN, reward % COIN, total_reward / COIN, total_reward % COIN, text, indicator ? ",<---" : "");
}
