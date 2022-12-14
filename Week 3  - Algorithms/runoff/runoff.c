#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
//Match candidates[i], where candidate[0] is the first one in candidate array,
//with the name variable which is where user vote is stored
//Every time we look at a voter ballot, its going to have his preferences and a name in each of those preferences.
bool vote(int voter, int rank, string name)
{
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (strcmp(candidates[i].name, name) == 0)
            {
                //updates preference array by putting name index at each location of preference array.
                preferences[voter][rank] = i;
                return true;
            }
        }
        return false;
    }
}
// Add votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //if candidate not yet elimiminated update vote, preferences[i][j] is index of candidates, ppreferences[i][j] = i
            if (candidates[preferences[i][j]].eliminated == false)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
    return;

}


// Print the winner of the election, if there is one
//If any candidate has more than half of the vote, their name should be printed and the function should return true.
//If nobody has won the election yet, the function should return false.
//Recall that voter_count stores the number of voters in the election.

bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > (voter_count / 2))
        {
            printf("%s\n", candidates[i].name);
            return true;
        }

    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int FewestVotes = candidates[0].votes;
    for (int i = 0; i < candidate_count; i++)
    {
        //making sure candidate is not eliminated and has more votes than FewestVotes,
        //which represents the number of votes the candiate with fewest votes has, as loop is ran
        if ((candidates[i].eliminated == false) && (candidates[i].votes < FewestVotes))
        {
            FewestVotes = candidates[i].votes;
        }
    }
    return FewestVotes;
}

// Return true if the election is tied between all candidates, false otherwise
// min- minimum number of votes that anyone in the election currently has.
bool is_tie(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        //The function should return true if every candidate remaining in the election has the same number of votes, and should return false otherwise.
        if ((candidates[i].eliminated == false) && (candidates[i].votes != min))
        {
            return false;
        }
    }
    //if all candidate tied return true
    return true;
}

// Eliminate the candidate (or candidates) in last place
//The function takes an argument min, which will be the minimum number of votes that anyone in the election currently has.
//The function should eliminate the candidate (or candidates) who have min number of votes.
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}