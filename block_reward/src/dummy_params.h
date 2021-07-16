typedef int64_t CAmount;

void LogPrintf(const char *format, ...)
{
	(void)format;
}

static const CAmount COIN = 100000000;
static const CAmount CENT = 1000000;

/** No amount larger than this (in satoshi) is valid.
 * 
 * Note that this constant is *not* the total money supply, which in DigiByte
 * currently happens to be less than 21,000,000 DGB for various reasons, but
 * rather a sanity check. As this sanity check is used by consensus-critical
 * validation code, the exact value of the MAX_MONEY constant is consensus
 * critical; in unusual circumstances like a(nother) overflow bug that allowed
 * for the creation of coins out of thin air modification could lead to a fork.
 * */
static const CAmount MAX_MONEY = 21000000000 * COIN;

#define BLOCK_TIME_SECONDS 15
#define MINUTES 60
#define SECONDS 60
#define HOURS 24
#define MONTHS_PER_YEAR 12
#define DAYS_PER_YEAR 365
#define SECONDS_PER_MONTH (SECONDS * MINUTES * HOURS * DAYS_PER_YEAR / MONTHS_PER_YEAR);


/**
 * Parameters that influence chain 
 */
class Consensus
{
public:
	struct Params {
	public:
		Params(void)
		{
			nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
			nPowTargetSpacing = 60 / 4;
				
			nTargetTimespan =  0.10 * 24 * 60 * 60; // 2.4 hours
			nTargetSpacing = 60; // 60 seconds
			nInterval = nTargetTimespan / nTargetSpacing;
			nDiffChangeTarget = 67200; // DigiShield Hard Fork Block BIP34Height 67,200
				
				// Old 1% monthly DGB Reward before 15 secon block change
			patchBlockRewardDuration = 10080; //10080; - No longer used
				//4 blocks per min, x60 minutes x 24hours x 14 days = 80,160 blocks for 0.5% reduction in DGB reward supply - No longer used
			patchBlockRewardDuration2 = 80160; //80160;
			nTargetTimespanRe = 1*60; // 60 Seconds
			nTargetSpacingRe = 1*60; // 60 seconds
			nIntervalRe = nTargetTimespanRe / nTargetSpacingRe; // 1 block
				
			nAveragingInterval = 10; // 10 blocks
			multiAlgoTargetSpacing = 30*5; // NUM_ALGOS * 30 seconds
			multiAlgoTargetSpacingV4 = 15*5; // NUM_ALGOS * 15 seconds
			nAveragingTargetTimespan = nAveragingInterval * multiAlgoTargetSpacing; // 10* NUM_ALGOS * 30
			nAveragingTargetTimespanV4 = nAveragingInterval * multiAlgoTargetSpacingV4; // 10 * NUM_ALGOS * 15
				
			nMaxAdjustDown = 40; // 40% adjustment down
			nMaxAdjustUp = 20; // 20% adjustment up
			nMaxAdjustDownV3 = 16; // 16% adjustment down
			nMaxAdjustUpV3 = 8; // 8% adjustment up
			nMaxAdjustDownV4 = 16;
			nMaxAdjustUpV4 = 8;

			nMinActualTimespan = nAveragingTargetTimespan * (100 - nMaxAdjustUp) / 100;
			nMaxActualTimespan = nAveragingTargetTimespan * (100 + nMaxAdjustDown) / 100;
			nMinActualTimespanV3 = nAveragingTargetTimespan * (100 - nMaxAdjustUpV3) / 100;
			nMaxActualTimespanV3 = nAveragingTargetTimespan * (100 + nMaxAdjustDownV3) / 100;
			nMinActualTimespanV4 = nAveragingTargetTimespanV4 * (100 - nMaxAdjustUpV4) / 100;
			nMaxActualTimespanV4 = nAveragingTargetTimespanV4 * (100 + nMaxAdjustDownV4) / 100;

			nLocalTargetAdjustment = 4; //target adjustment per algo
			nLocalDifficultyAdjustment = 4; //difficulty adjustment per algo
				
				// DigiByte Hard Fork Block Heights
			multiAlgoDiffChangeTarget = 145000; // Block 145,000 MultiAlgo Hard Fork
			alwaysUpdateDiffChangeTarget = 400000; // Block 400,000 MultiShield Hard Fork
			workComputationChangeTarget = 1430000; // Block 1,430,000 DigiSpeed Hard Fork
			algoSwapChangeTarget = 9100000; // Block 9,100,000 Odo PoW Hard Fork
				
			fPowAllowMinDifficultyBlocks = false;
			fPowNoRetargeting = false;
			nRuleChangeActivationThreshold = 28224; // 28224 - 70% of 40320
			nMinerConfirmationWindow = 40320; // nPowTargetTimespan / nPowTargetSpacing 40320 main net - 1 week
			fRbfEnabled = false;
				
			nOdoShapechangeInterval = 10*24*60*60; // 10 days
		}

		int nSubsidyHalvingInterval;
		/* Block hash that is excepted from BIP16 enforcement */
		/** Block height and hash at which BIP34 becomes active */
		int BIP34Height;
		/** Block height at which BIP65 becomes active */
		int BIP65Height;
		/** Block height at which BIP66 becomes active */
		int BIP66Height;
		int64_t workFinalBlockSubsidy = 40265288;

		/**
		* Minimum blocks including miner confirmation of the total of 2016 blocks in a retargeting period,
		* (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
		* Examples: 1916 for 95%, 1512 for testchains.
		*/
		uint32_t nRuleChangeActivationThreshold;
		uint32_t nMinerConfirmationWindow;
		/** Proof of work parameters */
		bool fPowAllowMinDifficultyBlocks;
		bool fPowNoRetargeting;
		bool fRbfEnabled;
		int64_t nPowTargetSpacing;
		int64_t nPowTargetTimespan;
		int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }
		
		int64_t nTargetTimespan;
		int64_t nTargetSpacing;
		int64_t nInterval;
		int64_t nDiffChangeTarget;
		int64_t nTargetTimespanRe;
		int64_t nTargetSpacingRe;
		int64_t nIntervalRe;
		int64_t patchBlockRewardDuration;
		int64_t patchBlockRewardDuration2;
		
		int64_t nAveragingInterval;
		int64_t multiAlgoTargetSpacing;
		int64_t multiAlgoTargetSpacingV4;
		int64_t nAveragingTargetTimespan;
		int64_t nAveragingTargetTimespanV4;
		
		int64_t nMaxAdjustDown;
		int64_t nMaxAdjustUp;
		int64_t nMaxAdjustDownV3;
		int64_t nMaxAdjustUpV3;
		int64_t nMaxAdjustDownV4;
		int64_t nMaxAdjustUpV4;
		
		int64_t nMinActualTimespan;
		int64_t nMaxActualTimespan;
		int64_t nMinActualTimespanV3;
		int64_t nMaxActualTimespanV3;
		int64_t nMinActualTimespanV4;
		int64_t nMaxActualTimespanV4;
		
		int64_t nLocalTargetAdjustment;
		int64_t nLocalDifficultyAdjustment;
		
		int64_t multiAlgoDiffChangeTarget;
		int64_t alwaysUpdateDiffChangeTarget;
		int64_t workComputationChangeTarget;
		int64_t algoSwapChangeTarget;
		
		uint32_t nOdoShapechangeInterval;

		bool EnableRBF() const { return fRbfEnabled; }
	};
};
