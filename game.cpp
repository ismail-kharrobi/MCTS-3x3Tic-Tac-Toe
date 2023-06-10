#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include "Board.hpp"

using namespace std;

class Node
{
    public :
        double visits = 0, uct = 0, wins = 0, draws = 0, losses = 0;
        Node *parent = nullptr;
        int winner = EMPTY;
        vector<Node*> childrens;
        Board state;
        cords move;
        int player;
       
        Node(){};
        Node(Board state , int player) : state(state) , player(player) {};
        void setUct()
        {
            if (this->visits == 0)
                this->uct = numeric_limits<double>::max();
            else
                this->uct = (((this->draws + this->wins) / 2) / this->visits ) + sqrt(2) * sqrt(log(parent->visits) / this->visits);
        }
        void addChildren(Node  *child)
        {
            this->childrens.push_back(child);
            child->parent = this;
        }

};

class Mcts
{
    public :
        Node *root_node = nullptr;
        Node *best_node = nullptr;
        Mcts() {};
        void deallocateNodes(Node* node)
        {
            if (node == nullptr)
                return;
            if (node->childrens.empty())
                return;
            for (Node* child : node->childrens)
            {
                deallocateNodes(child);
                delete child;
            }

            node->childrens.clear();
        }
        ~Mcts() {
            deallocateNodes(root_node);
            delete root_node;
        }
        void generateChildren(Node* n)
        {
            if (n == nullptr)
                return;
            vector<cords> moves = n->state.possibleMoves();
            if (moves.empty())
                return;
            
            if (moves.size() > 1)
            {
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(moves.begin() , moves.end(),g);
            }
            for (size_t i = 0; i < moves.size(); i++)
            {
                Board tmp_state(n->state);  
                int p = 1 - n->player;
                tmp_state.board[moves[i].x][moves[i].y] = p;
                Node* child = new Node(tmp_state, p);
                child->winner = child->state.isEnd();
                child->move = moves[i];
                n->addChildren(child);
            }
        }
        Node* selection() 
        {
            Node* current_node = root_node;
            if (root_node == nullptr)
                return nullptr;

            while (true)
            {
                if (current_node->state.isEnd() != EMPTY || current_node->state.isFull())
                    return current_node;

                if (current_node->childrens.empty())
                {
                    generateChildren(current_node);
                    return current_node->childrens[0];
                }
                else
                {
                    for (Node* child : current_node->childrens)
                    {
                        child->setUct();
                    }
                    std::sort(current_node->childrens.begin(), current_node->childrens.end(),
                            [](const Node* a, const Node* b) { return a->uct > b->uct; });
                    current_node = current_node->childrens[0];
                    if (current_node->visits == 0)
                        return current_node;
                    else if (current_node->childrens.empty())
                        return current_node;
                }
            }

        }
        void backPropagate(Node *node , int won)
        {
            Node *current = node;
            while (current != nullptr)
            {
                current->visits++;
                if (won == EMPTY)
                    current->draws++;
                else if (won == current->player)
                    current->wins++;
                else if (won != current->player)
                    current->losses++;
                current = current->parent;
            }
        }
        cords bestMove()
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            auto endTime = startTime + std::chrono::milliseconds(100);
            int playouts = 0;
            while (std::chrono::high_resolution_clock::now() < endTime)
            {
                playouts++;
                Node *leaf_to_roulout = selection();
                Board save_state = leaf_to_roulout->state;
                int won = leaf_to_roulout->state.simulate(leaf_to_roulout->player);
                leaf_to_roulout->state = save_state;
                backPropagate(leaf_to_roulout,won);
            }
            double visits_count = 0;
            for (Node* child : this->root_node->childrens)
            {
                if (child->visits > visits_count)
                {
                    this->best_node = child;
                    visits_count = child->visits;
                }
            }

            cout << "bestnode found draws : " << best_node->draws << " wins : " << best_node->wins << " loses : " << best_node->losses << " uct : " << best_node->uct << endl;
            cout << "correct move is : " << best_node->move.x << " " <<  best_node->move.y << " rollouts " << playouts << endl;
            return best_node->move;
        }
};


int main()
{

    Board board;
    while (true)
    {
        Mcts mcts;
        mcts.root_node = new Node(board , ENEMY);
        cords pos = mcts.bestMove();
        board.board[pos.x][pos.y] = ENEMY;
        printBoard(board);
        if (board.isEnd() != EMPTY || board.isFull())
            break;
        cout << "\nchose one : ";
        int play;
        cin >> play;
        pos = convertXY(play);
        board.board[pos.x][pos.y] = ME;
        cout << board.isEnd() << " | " << board.isFull() << endl;
        if (board.isEnd() != EMPTY || board.isFull())
            break ;
    }
    int winner = board.isEnd();
    if (winner != EMPTY)
    {
        cout << "Winner is "; player(winner); cout <<endl; 
    }
    else
        cout << "Draw!!\n";
}
