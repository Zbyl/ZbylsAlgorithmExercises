// SWTest-Orcs.cpp : Defines the entry point for the console application.
//

#include <iostream>

/// @brief State describe one state of the game.
///
/// In forward traversal (naive approach):
///   cost    - cost of getting to this state
///   orcs0   - number of hired orcs that never fought
///   orcs1   - number of hired orcs that fought once
///   orcs2   - number of hired orcs that fought twice
///
/// In backward traversal:
///   cost    - minimal cost of getting from this state to the end state (on the path we chose)
///   orcs0   - minimal number of hired orcs that never fought, that are needed to reach end state
///   orcs1   - minimal number of hired orcs that fought once, that are needed to reach end state
///   orcs2   - minimal number of hired orcs that fought twice, that are needed to reach end state
struct State
{
    State(int cost, int orcs0, int orcs1, int orcs2)
        : cost(cost)
        , orcs0(orcs0)
        , orcs1(orcs1)
        , orcs2(orcs2)
    {}

    int cost;
    int orcs0;
    int orcs1;
    int orcs2;

    bool haveOrcs() const
    {
        return (orcs0 != 0) || (orcs1 != 0) || (orcs2 != 0);
    }
};

struct Turn
{
    int cost;   ///< Cost of bribing the orcs; half of cost of hiring the orcs.
    int orcs;   ///< Number of orcs that we meet in given turn.
};

/// @brief Expands given state one turn forwards.
/// @param state                Minimal state, that is needed to reach the final state, following our path.
/// @param turnCount            Number of turns.
/// @param turnIdx              Current turn index (equal to turnCount for the final state).
/// @param turns                Array of turns, of size turnCount.
/// @param minCost              Minimal cost of reaching final state. (-1 if not known yet)
void traverseForward(const State& state, int turnCount, int turnIdx, const Turn turns[], int& minimalCost)
{
    if ( (minimalCost != -1) && (state.cost >= minimalCost) )
        return;

    if (turnIdx == turnCount)
    {
        // we have reached the final state

        if ((minimalCost == -1) || (minimalCost > state.cost))
        {
            minimalCost = state.cost;
        }

        return;
    }

    Turn turn = turns[turnIdx];

    // first we try to go through "pay" option
    State payState(state.cost + turn.cost, state.orcs0, state.orcs1, state.orcs2);
    traverseForward(payState, turnCount, turnIdx + 1, turns, minimalCost);

    // now we try to go through "fight" option
    {
        State fightState(state.cost, state.orcs0, state.orcs1, state.orcs2);

        // we need to remove orcs killed in battle, veterans first
        fightState.orcs2 -= turn.orcs;
        if (fightState.orcs2 < 0)
        {
            fightState.orcs1 += fightState.orcs2;
            fightState.orcs2 = 0;

            if (fightState.orcs1 < 0)
            {
                fightState.orcs0 += fightState.orcs1;
                fightState.orcs1 = 0;
            }
        }

        // check if we had enough orcs to fight
        if (fightState.orcs0 >= 0)
        {
            // we need to make orcs that fought veterans
            fightState.orcs2 = fightState.orcs1;
            fightState.orcs1 = fightState.orcs0;
            fightState.orcs0 = 0;
            traverseForward(fightState, turnCount, turnIdx + 1, turns, minimalCost);
        }
    }

    // now we try to go through "hire" option
    // we check hire state last, because it generates highest costs (we hope it will be cut off by expanding fight and pay nodes first)
    State hireState(state.cost + turn.cost * 2, state.orcs0 + turn.orcs, state.orcs1, state.orcs2);
    traverseForward(hireState, turnCount, turnIdx + 1, turns, minimalCost);
}

/// @brief Finds best cost for the whole path. Uses naive, forward traversal.
/// @param turnCount            Number of turns.
/// @param turns                Array of turns, of size turnCount.
/// @returns Best cost for traversing the path.
int forwardTraversal(int turnCount, const Turn turns[])
{
    State firstState(0, 0, 0, 0);
    int minimalCost = -1;
    traverseForward(firstState, turnCount, 0, turns, minimalCost);

    return minimalCost;
}

/// @brief Expands given state one turn backwards.
/// @param state                Minimal state, that is needed to reach the final state, following our path.
/// @param turnIdx              Current turn index (equal to turnCount for the final state).
/// @param turns                Array of turns, of size turnCount.
/// @param orcFreeTurnCosts     Minimal cost for each turn, needed to reach final state from this one, without having any orcs. Array of size turnCount + 1. (-1 if not known yet)
void traverseBackward(const State& state, int turnIdx, const Turn turns[], int orcFreeTurnCosts[])
{
    int minimalCost = orcFreeTurnCosts[turnIdx];
    if ((minimalCost != -1) && (state.cost >= minimalCost))
    {
        // we can reach final state using lower cost, and without any orcs; so our current path is definitely worse; we skip it
        return;
    }

    if (!state.haveOrcs())
    {
        // if we have no orcs, then update our best cost
        orcFreeTurnCosts[turnIdx] = state.cost;
    }

    if (turnIdx == 0)
    {
        // we have reached the end
        // we already updated the best cost, if we have no orcs (otherwise this path is not valid)
        return;
    }

    Turn turn = turns[turnIdx - 1];
    
    // first we try to go backwards through "pay" option
    State payState(state.cost + turn.cost, state.orcs0, state.orcs1, state.orcs2);
    traverseBackward(payState, turnIdx - 1, turns, orcFreeTurnCosts);

    // now we try to go backwards through "hire" option
    // but if we don't need any orcs, we don't need to hire them (the "pay" path will always be better)
    if (state.haveOrcs())
    {
        State hireState(state.cost + turn.cost * 2, state.orcs0, state.orcs1, state.orcs2);
        // we need to modify the state, to account for the hired orcs
        hireState.orcs0 -= turn.orcs;
        if (hireState.orcs0 < 0)
        {
            hireState.orcs1 += hireState.orcs0;
            hireState.orcs0 = 0;

            if (hireState.orcs1 < 0)
            {
                hireState.orcs2 += hireState.orcs1;
                hireState.orcs1 = 0;

                if (hireState.orcs2 < 0)
                {
                    // in current state we need even less orcs, than we can buy here, so we just clamp to 0
                    hireState.orcs2 = 0;
                }
            }
        }
        traverseBackward(hireState, turnIdx - 1, turns, orcFreeTurnCosts);
    }

    // now we try to go backwards through "fight" option
    // but if in the current state we need any orcs that didn't fight, then we can't go this path
    if (state.orcs0 == 0)
    {
        // we need at the very least additional turn.orcs of orcs, and they can be veterans
        State fightState(state.cost, state.orcs1, state.orcs2, turn.orcs);
        traverseBackward(fightState, turnIdx - 1, turns, orcFreeTurnCosts);
    }
}

/// @brief Finds best cost for the whole path.
/// @param turnCount            Number of turns.
/// @param turns                Array of turns, of size turnCount.
/// @param orcFreeTurnCosts     Minimal cost for each turn, needed to reach final state from this one, without having any orcs. Array of size turnCount + 1. (-1 if not known yet)
/// @returns Best cost for traversing the path.
int backwardTraversal(int turnCount, const Turn turns[], int orcFreeTurnCosts[])
{
    for (int i = 0; i <= turnCount; ++i)
        orcFreeTurnCosts[i] = -1;

    State lastState(0, 0, 0, 0);
    traverseBackward(lastState, turnCount, turns, orcFreeTurnCosts);

    return orcFreeTurnCosts[0];
}

const int MAX_TURNS = 20;
Turn gTurns[MAX_TURNS];
int gOrcFreeTurnCosts[MAX_TURNS + 1];

int main()
{
#ifdef ZBYL
    //freopen("SWTest-find-Orcs-input.txt", "r", stdin);
    freopen("sample_input.txt", "r", stdin);
#endif

    int T;
    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        int N;
        std::cin >> N;

        for (int n = 0; n < N; ++n)
        {
            int c, o;
            std::cin >> o >> c;
            gTurns[n].cost = c;
            gTurns[n].orcs = o;
        }

        int minCost0 = forwardTraversal(N, gTurns);
        int minCost1 = backwardTraversal(N, gTurns, gOrcFreeTurnCosts);
        if (minCost0 != minCost1)
            std::cout << "#" << (t + 1) << " Bug." << std::endl;
        std::cout << "#" << (t + 1) << " " << minCost0 << std::endl;
    }

    return 0;
}

