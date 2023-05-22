#include "beta.h"
using namespace std;
using namespace std::chrono;

int position_matrix[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 3, 2, 1, 0,
	0, 2, 4, 4, 4, 4, 2, 0,
	0, 3, 4, 5, 5, 4, 3, 0,
	0, 3, 4, 5, 5, 4, 3, 0,
	0, 2, 4, 4, 4, 4, 2, 0,
	0, 1, 2, 3, 3, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

bool comparator(U64 x, U64 y) {
	return (position_matrix[get_move_to(x)] + get_move_capture(x) * 100 > position_matrix[get_move_to(y)] + get_move_capture(y) * 100);
}

int Evaluation(U64 b2[13]) {

	int evaluation = 0;

	//
	//duration<double> time;
	//time_point<chrono::high_resolution_clock> start_time;
	//start_time = high_resolution_clock::now();
	//

	/*for (int i = 0; i < 64; i++) {
		if (b2[1] & 1ULL << i) evaluation += 1000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[2] & 1ULL << i) evaluation += 3000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[3] & 1ULL << i) evaluation += 3000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[4] & 1ULL << i) evaluation += 5000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[5] & 1ULL << i) evaluation += 9000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[6] & 1ULL << i) evaluation += 100000 + position_matrix[i];
	}



	for (int i = 0; i < 64; i++) {
		if (b2[7] & 1ULL << i) evaluation -= 1000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[8] & 1ULL << i) evaluation -= 3000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[9] & 1ULL << i) evaluation -= 3000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[10] & 1ULL << i) evaluation -= 5000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[11] & 1ULL << i) evaluation -= 9000 + position_matrix[i];
	}

	for (int i = 0; i < 64; i++) {
		if (b2[12] & 1ULL << i) evaluation -= 100000 + position_matrix[i];
	}*/

	evaluation += 1000 * popcount(b2[1]);
	evaluation += 3000 * popcount(b2[2]);
	evaluation += 3000 * popcount(b2[3]);
	evaluation += 5000 * popcount(b2[4]);
	evaluation += 9000 * popcount(b2[5]);
	evaluation += 100000 * popcount(b2[6]);

	evaluation -= 1000 * popcount(b2[7]);
	evaluation -= 3000 * popcount(b2[8]);
	evaluation -= 3000 * popcount(b2[9]);
	evaluation -= 5000 * popcount(b2[10]);
	evaluation -= 9000 * popcount(b2[11]);
	evaluation -= 100000 * popcount(b2[12]);

	//
	//time = high_resolution_clock::now() - start_time;
	//cout << time.count() << "\n";
	//

	//4e-07 -> 1e-06


	return evaluation;
}

int quiescence(U64 b2[13], int depth, int alpha, int beta, int colour) {

	if (!depth) return Evaluation(b2);


	int minEval = Evaluation(b2);
	int maxEval = minEval;

	int evaluation;

	vector<U64> moves;
	gen_captures(b2, colour, moves);
	sort(moves.begin(), moves.end(), comparator);


	if (colour) {

		for (int i = 0; i < moves.size(); i++) {

			make_move(b2, moves[i], colour);
			evaluation = quiescence(b2, depth - 1, alpha, beta, 0);
			undo_move(b2, moves[i], colour);


			if (evaluation > maxEval) maxEval = evaluation;


			// alpha-beta pruning
			alpha = max(alpha, evaluation);
			if (beta <= alpha) break;
		}

		return maxEval;
	}


	else {

		for (int i = 0; i < moves.size(); i++) {

			make_move(b2, moves[i], colour);
			evaluation = quiescence(b2, depth - 1, alpha, beta, 1);
			undo_move(b2, moves[i], colour);


			if (evaluation < minEval) minEval = evaluation;


			// alpha-beta pruning
			beta = min(beta, evaluation);
			if (beta <= alpha) break;
		}

		return minEval;
	}
}

int minimax(U64 b2[13], int depth, int alpha, int beta, int colour) {

	if (!depth) return quiescence(b2, 4, alpha, beta, colour);


	int minEval = 999999;
	int maxEval = -999999;
	int evaluation;

	vector<U64> moves;
	gen_moves(b2, colour, moves);
	sort(moves.begin(), moves.end(), comparator);


	if (colour) {

		for (int i = 0; i < moves.size(); i++) {

			//cout << moves[i] << " " << get_move_capture(moves[i]) << "\n";

			make_move(b2, moves[i], colour);
			evaluation = minimax(b2, depth - 1, alpha, beta, 0);
			undo_move(b2, moves[i], colour);


			if (evaluation > maxEval) maxEval = evaluation;


			// alpha-beta pruning
			alpha = max(alpha, evaluation);
			if (beta <= alpha) break;
		}

		//cout << "\n\n";
		return maxEval;
	}


	else {

		for (int i = 0; i < moves.size(); i++) {

			make_move(b2, moves[i], colour);
			evaluation = minimax(b2, depth - 1, alpha, beta, 1);
			undo_move(b2, moves[i], colour);


			if (evaluation < minEval) minEval = evaluation;


			// alpha-beta pruning
			beta = min(beta, evaluation);
			if (beta <= alpha) break;
		}

		return minEval;
	}
}

int first_layer_minimax(Board& b, int depth, int colour) {

	int alpha = -9999999;
	int beta = 9999999;

	int minEval = 999999;
	int maxEval = -999999;
	int evaluation;

	vector<U64> moves;
	gen_moves(b.b2, colour, moves);
	sort(moves.begin(), moves.end(), comparator);

	int count = 1;
	if (colour) {

		for (int i = 0; i < moves.size() ; i++) {

			cout << count++ << " ";

			make_move(b.b2, moves[i], colour);
			evaluation = minimax(b.b2, depth - 1, alpha, beta, 0);
			undo_move(b.b2, moves[i], colour);


			if (evaluation > maxEval) {
				maxEval = evaluation;
				b.n_move = moves[i];
			}


			// alpha-beta pruning
			alpha = max(alpha, evaluation);
			if (beta <= alpha) break;
		}

		cout << "}\n";
		return maxEval;
	}


	else {

		for (int i = 0; i < moves.size(); i++) {

			cout << count++ << " ";

			make_move(b.b2, moves[i], colour);
			evaluation = minimax(b.b2, depth - 1, alpha, beta, 1);
			undo_move(b.b2, moves[i], colour);


			if (evaluation < minEval) {
				minEval = evaluation;
				b.n_move = moves[i];
			}


			// alpha-beta pruning
			beta = min(beta, evaluation);
			if (beta <= alpha) break;
		}

		cout << "}\n";
		return minEval;
	}
}

int iterative_deepening(Board& b, int colour, double max_time) {

	time_point<chrono::high_resolution_clock> start_time;
	duration<double> time;

	int evaluation = 0;
	int depth = 1;


	while (true) {

		cout << depth << " { ";
		start_time = high_resolution_clock::now();


		evaluation = first_layer_minimax(b, depth, colour);


		if ((time = high_resolution_clock::now() - start_time).count() > max_time) {
			cout << "\n";
			return evaluation;
		}

		depth++;
	}
}