//
// Created by Jeremy S on 2017-11-26.
//

#pragma once

#include <iostream>
#include "../game/Player.h"
#include "framework/entity_t.hpp"
#include "syscl.hpp"

namespace rl {

    using player_t = Player;

    class ai_player : public player_t, public ml::entity_t {
    private:

        size_t num_turns = 0;

        double points = 0;

    public:

        explicit ai_player(ptr<ml::network_o> net)
                : player_t(1),
                  entity_t(std::move(net))
        {}

        ai_player(int p_id, ptr<ml::network_o> net)
                : player_t(p_id),
                  entity_t(std::move(net))
        {}

        // Points are alloted as follows:
        //      win: +2
        //      tie: +1
        //      performance: t/12 - l/12
        //          the percentage of pieces taken minus the
        //          percentage of pieces lost
        //
        // Determining result based off of points:
        //      t = tie, w = win
        //
        //      !t &  w -> p in (2, 3]
        //       t &  w -> p in (1, 2]
        //       t & !w -> p in [0, 1)
        //      !t & !w -> p in [-1,0)
        //
        //       p == 1 -> complete tie
        //       p == 3 -> perfect win (no pieces lost)
        void process_results (cl::game_data const &result) {
            // win condition points
            if (result.winner == this->player && !result.tie) {
                points += 2;
            } else if (result.tie) {
                points += 1;
            }

            // performance points
            if (this->player == 1) {
                points += (double(result.p1taken) / 12.0) - (double(result.p2taken / 12.0));
                // std::cout << "Pieces taken " << result.p1taken << std::endl;
                // std::cout << "Pieces lost  " << result.p2taken << std::endl << std::endl; 
            } else {
                points += (double(result.p2taken) / 12.0) - (double(result.p1taken / 12.0));
                // std::cout << "Pieces taken " << result.p2taken << std::endl;
                // std::cout << "Pieces lost  " << result.p1taken << std::endl << std::endl;
            }
        }

        unsigned int index(int row, int col);

        void getMove (std::vector<std::vector<Piece>> const &gameBoard) override;

        void reset () override {
            points = 0;
        }

        double get_value () const override {
            return points;
        }

    };

    inline unsigned int ai_player::index(int row, int col) {
        unsigned int pos = (unsigned int)(row*4 + col/2);
        if (this->player == 2) pos = 31-pos;
        return pos;
    }

    inline void ai_player::getMove(std::vector<std::vector<Piece>> const &gameBoard) {
        if (possibleMoves.size() == 0) {
            canMove = false;
            return;
        }
        movesToString();
        // std::cout << "Possible Moves: ";
        // for (auto s : movesAsString) {
        //     std::cout << s << " ";
        // }
        // std::cout << std::endl;

        //format inputs for neural network
        std::array<double,NEURAL_IN> inputs{};
        for (int row=0; row<8; row++) {
            for (int col=1-row%2; col<8; col+=2) {
                Piece piece = gameBoard[row][col];

                //if my piece on this tile, player = 1
                //if opponent piece on tile, player = -1
                double ismine = 2 * (player == piece.player) - 1;

                if (piece.isEmpty) {
                    inputs[index(row, col)] = 0;
                } else {
                    inputs[index(row, col)] = ismine;
                    if (piece.isKing)
                        inputs[32 + index(row, col)] = ismine;
                }
            }
        }
        inputs[64] = double(num_turns) / 75.0;
        num_turns++;

        //ask the AI
        std::array<double,NEURAL_OUT> outputs = network->solve(inputs);

        //pick the dest with the highest fitness value,
        //  and the orig with the lowest fitness value
        double dest_fitness = -1337;
        double orig_fitness = 1337;

        for (size_t i=0; i<possibleMoves.size(); i++) {
            //get fitness values of move start and move end
            Point &moveend = possibleMoves[i].back();
            Point &movestart = possibleMoves[i].front();
            double fitend = outputs[index(moveend.row, moveend.col)];
            double fitstart = outputs[index(movestart.row, movestart.col)];

            if (dest_fitness < fitend) {        //if current move end is more desirable
                dest_fitness = fitend;              //set current move
                orig_fitness = fitstart;
                indexOfMove = (int)i;
            } else if (dest_fitness == fitend) {
                if (orig_fitness > fitstart) {  //if current move start is more desirable
                    orig_fitness = fitstart;        //set current move
                    indexOfMove = (int)i;
                }
            }
        }
    }

} // namespace rl
