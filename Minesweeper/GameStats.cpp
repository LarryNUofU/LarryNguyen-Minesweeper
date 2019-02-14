#include "GameStats.h"

GameStats::GameStats()
{
	resetStats();
}

GameStats::~GameStats()
{
}

void GameStats::resetStats()
{
	easyGamesWon = 0;
	easyGamesLost = 0;
	medGamesWon = 0;
	medGamesLost = 0;
	expertGamesWon = 0;
	expertGamesLost = 0;

	fastestEasyWon = DBL_MAX;
	fastestMedWon = DBL_MAX;
	fastestExpertWon = DBL_MAX;
}

void GameStats::loadStats()
{
	GameStats stats;
	{
		// create and open an archive for input
		std::ifstream ifs("save.dat");
		boost::archive::text_iarchive ia(ifs);
		// read class state from archive
		ia >> stats;
		// archive and stream closed when destructors are called
	}

	easyGamesWon = stats.easyGamesWon;
	easyGamesLost = stats.easyGamesLost;
	medGamesWon = stats.medGamesWon;
	medGamesLost = stats.medGamesLost;
	expertGamesWon = stats.expertGamesWon;
	expertGamesLost = stats.expertGamesLost;

	fastestEasyWon = stats.fastestEasyWon;
	fastestMedWon = stats.fastestMedWon;
	fastestExpertWon = stats.fastestExpertWon;
}

//0 = easy mode, 1 = medium mode, 2 = expert mode
void GameStats::saveStats(int difficultySelector, double duration)
{
	if (difficultySelector == 0)
	{
		easyGamesWon++;

		if (duration < fastestEasyWon)
			fastestEasyWon = duration;

	}
	else if (difficultySelector == 1)
	{
		medGamesWon++;
		if (duration < fastestMedWon)
			fastestMedWon = duration;
	}	
	else
	{
		expertGamesWon++;
		if (duration < fastestExpertWon)
			fastestExpertWon = duration;
	}

	std::ofstream ofs("save.dat");

	// save data to archive
	{
		boost::archive::text_oarchive oa(ofs);
		// write class instance to archive
		oa << *this;
		// archive and stream closed when destructors are called
	}
}

bool GameStats::saveFileExists()
{
	const char* name = "save.dat";
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}