#include <math.h>

//from https://en.wikipedia.org/wiki/Elo_rating_system

namespace elo {
	//Main elo calculation function.
	//Input:
	//	double player	-- the player's rating
	//	double opponent	-- their opponent's rating
	//	double score 	-- a value between 0 and 1, the score of the game
	//Output:
	//	The updated player rating after the match.
	double update(double player, double opponent, double score) {
		return player + k_factor(player) * (score - expected(player, opponent));
	}
	
	double update_raw(double player, double expected, double score) {
		return player + k_factor(player) * (score - expected);
	}
	
	//Elo calculation function, which directly updates the values of player/opponent ratings in memory.
	//Input:
	//	double *player		-- a pointer to the player's rating
	//	double *opponent	-- a pointer to their opponent's rating
	//	double score 		-- a value between 0 and 1, the score of the game
	//Output:
	//	Updates the ratings of the player/opponent in-place.
	//	Assumes that the opponent's score is 1 minus score:
	//		i.e. if player had a score of 1, opponent has a score of 0
	//			 if player had a score of 0.5, opponent has a score of 0.5
	double update(double *player, double *opponent, double score) {
		*player += k_factor(*player) * (score - expected(*player, *opponent));
		*opponent += k_factor(*opponent) * ((1-score) - expected(*opponent, *player));
	}
	
	
	
	
	//Given their elo ratings, returns the expected chance of a player winning in a match against an opponent.
	inline double expected(double player, double opponent) {
		return 1/(1 + pow(10, (opponent-player)/400));
	}
	
	//Returns the k-factor to use with a certain rating
	inline double k_factor(double rating) {
		return 32;
		//should go down with higher rating
	}
	
}