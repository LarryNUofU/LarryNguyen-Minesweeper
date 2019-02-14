#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <sys/stat.h>

class GameStats
{
public:
	GameStats();
	~GameStats();

	int easyGamesWon;
	int easyGamesLost;
	int medGamesWon;
	int medGamesLost;
	int expertGamesWon;
	int expertGamesLost;

	//represents the duration
	double fastestEasyWon;
	double fastestMedWon;
	double fastestExpertWon;


	void resetStats();
	void loadStats();
	void saveStats(int difficultySelector, double duration);
	bool saveFileExists();


private:
	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & easyGamesWon;
		ar & easyGamesLost;
		ar & medGamesWon;
		ar & medGamesLost;
		ar & expertGamesWon;
		ar & expertGamesLost;
		ar & fastestEasyWon;
		ar & fastestMedWon;
		ar & fastestExpertWon;
	}
};

