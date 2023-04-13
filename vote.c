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
bool vote(int voter, int rank, string name)
{
    bool valid_name = false;
    for (int i = 0; i < candidate_count; i++)   //Assign preferences[voter][rank] an value each rank
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            valid_name = true;
        }
    }
    return valid_name;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)   // Loop over every voter
    {
        for (int y = 0, s = 0; y < candidate_count; y++)    //Loop over every candidate
        {
            s = preferences[i][y];  // Capture the subsequent value of preferences[i][y] in s (i.e. the array index in rank, assigned in vote function)

            if (candidates[s].eliminated != true)   // If selected candidate is not eliminated, increment voter by 1, stop second loop and go to next voter
            {
                candidates[s].votes++;
                y = candidate_count;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    string winner;
    bool early_winner = false;
    for (int j = 0, l = 0; j < candidate_count; j++)    //Loop over each candidate, if there is only 1 candidate not elimanated, print his name
    {
        if (candidates[j].eliminated != true)
        {
            l++;
            winner = candidates[j].name;
        }
        if (j == candidate_count - 1 && l == 1)
        {
            printf("%s\n", winner);
        }
    }

    for (int i = 0, y = 0, t = 0; i < candidate_count; i++) //Loop over each candidate, if there is an early winner (i.e. having the majority of votes) print his name
    {
        if (candidates[i].eliminated != true && candidates[i].votes >= voter_count / 2 + 1)
        {
            y++;
            t = i;
        }
        if (i == candidate_count - 1 && t == 1)
        {
            early_winner = true;
            printf("%s\n", candidates[t].name);
            i = candidate_count;
        }
    }
    return early_winner;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int smallest_number = 0;
    for (int i = 0, y = 0; i < candidate_count; i++)    //Loop over each not eliminated candidate and return the smalles amount of voter on
    {
        if (candidates[i].eliminated != true
            && candidates[i].votes == y)
        {
            smallest_number = candidates[i].votes;
            i = candidate_count;
        }
        else if (i == candidate_count - 1)
        {
            i = -1;
            y++;
        }
    }
    return smallest_number;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int active_candidates = 0;
    bool tie = false;

    for (int t = 0; t < candidate_count; t++)   //Capture current amount of not eliminated candidates
    {
        if (candidates[t].eliminated != true)
        {
            active_candidates++;
        }
    }
    for (int i = 0, y = 0; i < candidate_count; i++)    //If there is a tie amound not eliminated candidates, return tie = true
    {
        if (candidates[i].votes == min && candidates[i].eliminated != true
            && active_candidates != 1)
        {
            y++;
        }
        if (y == active_candidates)
        {
            i = candidate_count;
            tie = true;
        }
    }
    return tie;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0, y = 0; i < candidate_count; i++)    //Loop over each candidate
    {
        if (candidates[i].eliminated != true && candidates[i].votes == min) //Evaluate if there is a tie and stop loop if there is
        {
            y++;
        }
        if (i == candidate_count - 1 && y == candidate_count - 1)
        {
            i = candidate_count;
        }
        else if (i == candidate_count - 1 && y != candidate_count - 1)  //If there is no tie start a new loop, loop over each candidate, eliminate evry candidate with "min" votes
        {
            for (int l = 0; l < candidate_count; l++)
            {
                if (candidates[l].eliminated != true && candidates[l].votes == min)
                {
                    candidates[l].eliminated = true;
                }
            }
        }
    }
}
