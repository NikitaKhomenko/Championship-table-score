#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAME_LENGTH 40

typedef struct game
{
    char name1[NAME_LENGTH]; // name of team 1
    char name2[NAME_LENGTH];// name of team 2
    int goals1;   // goals scored  by team 1
    int goals2;  // goals scored by team 2
} GAME;

typedef struct game_node
{
    GAME agame;
    struct game_node *next;
} GAME_NODE;


typedef struct team
{
    char name[NAME_LENGTH];
    int games_played;
    int wins;
    int draws;
    int losses;
    GAME_NODE *games;
} TEAM;

GAME* ReadGames(int *pngames);
TEAM* FillTable(int *pnum_teams, GAME all_games[], int num_games);
TEAM* PrepareTable(int* tsize, GAME allgames[], int num_games);
void  PrintTable(TEAM table[], int num_teams);

GAME ReadGame();
int findIfNameExist (TEAM all_teams[], const char* name, int size);
void addGameLists(TEAM table[], int num_teams, GAME allgames[], int num_games);
void fillRestOfTableTypes(TEAM table[], int num_teams);

/////// ***** MAIN ***** ///////

void main()
{
    GAME* all_games = NULL;
    TEAM* table = NULL;
    int num_games = 0, num_teams = 0;

    all_games = ReadGames(&num_games);
    table  = FillTable(&num_teams, all_games, num_games);




    PrintTable(table, num_teams);
//    FreeAllGames(all_games, num_games);
//    FreeAllTeams(table, num_teams);

}

    ////////////////////////////// ********* functions assigned to structs ********* //////////////////////////////

GAME* ReadGames(int *pngames)
{
    char choice = 'y';
    GAME* all_games = NULL;
    GAME game;

    while (choice != 'n')
    {
        printf("\nWould you like to add another game? (y/n) ");
        scanf(" %c", &choice);

        if (choice == 'y')
        {
            game = ReadGame();
            all_games = (GAME*)realloc(all_games, (*pngames + 1) * sizeof(GAME));
            all_games[*pngames] = game;
            (*pngames)++;
        }
        else if (choice != 'y' && choice != 'n')
        {
            printf("Wrong input.\n");
        }
    } //while

    return all_games;

}


TEAM* FillTable(int *pnum_teams, GAME all_games[], int num_games)
{
    TEAM* table = NULL;
    table = PrepareTable(pnum_teams, all_games, num_games);
    addGameLists(table, *pnum_teams, all_games, num_games);
    fillRestOfTableTypes(table, *pnum_teams);

    return table;
}

TEAM* PrepareTable(int* tsize, GAME allgames[], int num_games)
{
    TEAM* table = NULL;
    int nameExist, num_teams = 1;

    table = (TEAM*)malloc(sizeof(TEAM));
    strcpy(table[0].name, allgames[0].name1);

    // check every team if name already exist, increment game count every iteration.
    for (int gamesCount = 0; gamesCount < num_games; gamesCount++)
    {
        // finds first name in allgames array is already exists in table
        nameExist = findIfNameExist(table, allgames[gamesCount].name1, num_teams);
        if (nameExist == 0)
        {
            table = (TEAM*)realloc(table, ++num_teams * sizeof(TEAM));
            strcpy(table[num_teams-1].name, allgames[gamesCount].name1);

        }
        // finds second name in allgames array is already exists in table
        nameExist = findIfNameExist (table, allgames[gamesCount].name2, num_teams);
        if (nameExist == 0)
        {
            table = (TEAM*)realloc(table, ++num_teams * sizeof(TEAM));
            strcpy(table[num_teams-1].name, allgames[gamesCount].name2);
        }
    }
    *tsize = num_teams;
    return table;
}

void  PrintTable(TEAM table[], int num_teams)
{
    for (int teamCount = 0; teamCount < num_teams; ++teamCount) {
        printf("\n\n\nTeam Name: %s\n",table[teamCount].name);
        printf("Games Played: %d \t", table[teamCount].games_played);
        printf("Wins: %d \t", table[teamCount].wins);
        printf("Loses: %d \t", table[teamCount].losses);
        printf("Draws: %d \t", table[teamCount].draws);

        printf("\n\nGames List:\n");


    }
}




    ////////////////////////////// ********* assistance functions ********* //////////////////////////////

GAME ReadGame()
{
    GAME game;

    printf("Please enter first team: ");
    getchar();
    fgets(game.name1, NAME_LENGTH, stdin);
    printf("Please enter first team's goals: ");
    scanf("%d", &game.goals1);
    printf("Please enter second team: ");
    getchar();
    fgets(game.name2, NAME_LENGTH, stdin);
    printf("Please enter second team's goals: ");
    scanf("%d", &game.goals2);

    return game;
}

int findIfNameExist (TEAM all_teams[], const char *name, int size)
{
    int exist = 0;
    for (int i = 0; i < size; ++i) {
        if(strcmp(all_teams[i].name, name) == 0)
            exist = 1;
    }
        return exist;
}


void addGameLists(TEAM *table, int num_teams, GAME *allgames, int num_games)
{
    // declaring on array of game lists, each list for each team.
    int gamesPlayed;
    for (int teamsCount = 0; teamsCount < num_teams; ++teamsCount)
    {
        GAME_NODE *prevNode=0, *newNode=0;
        gamesPlayed = 0;
        for (int gamesCount = 0; gamesCount < num_games; ++gamesCount)
        {
            if (strcmp(allgames[gamesCount].name1, table[teamsCount].name) == 0
                    || strcmp(allgames[gamesCount].name2, table[teamsCount].name) == 0)
            {
                newNode = (GAME_NODE*)malloc(sizeof(GAME_NODE));
                newNode->agame = allgames[gamesCount];
                if (prevNode == 0)
                {
                    table[teamsCount].games = newNode;
                }
                else
                {
                    prevNode->next = newNode;
                }
                prevNode = newNode;
                ++gamesPlayed;
            }
        }
        table[teamsCount].games_played = gamesPlayed;     // fill games played for each team
    }

}

void fillRestOfTableTypes(TEAM *table, int num_teams)
{
    int home, out;  //scores
    int gamesPlayedByTeam;
    GAME_NODE *currentNode=0;

    for (int teamsCount = 0; teamsCount < num_teams; ++teamsCount)
    {
        gamesPlayedByTeam =  table[teamsCount].games_played;
        table[teamsCount].wins = 0;
        table[teamsCount].losses = 0;
        table[teamsCount].draws = 0;
        currentNode = table[teamsCount].games;

        for (int gamesCount = 0; gamesCount < gamesPlayedByTeam; ++gamesCount)
        {
            if (strcmp(table[teamsCount].games->agame.name1, table[teamsCount].name) == 0)
            {
                home = currentNode->agame.goals1;
                out = currentNode->agame.goals2;
            }else{
                home = currentNode->agame.goals2;
                out = currentNode->agame.goals1;
            }

            // filling table //

            if (home < out)
                ++(table[teamsCount].losses);

            else if (home > out)
                ++(table[teamsCount].wins);

            else
                ++(table[teamsCount].draws);

            currentNode = currentNode->next;
       }
    }
}
