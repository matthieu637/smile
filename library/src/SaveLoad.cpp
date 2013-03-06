#include "sml/SaveLoad.hpp"


#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>


namespace sml{

using namespace boost::interprocess;

void SaveLoad::syncQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION)
{

    named_mutex mutex( open_or_create, chemin.c_str());
    mutex.lock();

    FILE *fp = fopen("matrix.txt", "w+");

    //si le fichier n'existe pas, pas de sync
    fseek(fp, 0, SEEK_END);
    if( ftell(fp) > 0 )
    {
        for(int i = 0; i < STATES_ALPHA; i++)
            for(int j = 0; j < STATES_DISTANCE; j++)
                for(int k = 0; k < ACTIONS_ACC; k++)
                    for(int m = 0; m < ACTIONS_DIRECTION; m++)
                    {
                        float q;
                        fscanf(fp, "%f", &q);
                        if(q != 0.0 && Q[i][j][k][m] == 0.0)
                            Q[i][j][k][m] = q;
                        else if( q != 0.0 && Q[i][j][k][m] != 0.0 )
                            Q[i][j][k][m] = ( Q[i][j][k][m] + q*3. )/4.; //les valeurs déjà apprisent ont plus de poids
                    }
    }
    rewind(fp);

    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
            for(int k = 0; k < ACTIONS_ACC; k++)
                for(int m = 0; m < ACTIONS_DIRECTION; m++)
                    fprintf(fp, "%f\n", Q[i][j][k][m]);
    fclose(fp);

    mutex.unlock();
}


void SaveLoad::writeQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION)
{
    named_mutex mutex( open_or_create, chemin.c_str());
    mutex.lock();

    FILE *fp = fopen("matrix.txt", "w");
    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
            for(int k = 0; k < ACTIONS_ACC; k++)
                for(int m = 0; m < ACTIONS_DIRECTION; m++)
                    fprintf(fp, "%f\n", Q[i][j][k][m]);
    fclose(fp);
    mutex.unlock();
}


void SaveLoad::readQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION)
{
    named_mutex mutex( open_or_create, chemin.c_str());
    mutex.lock();
    
    FILE *fp = fopen("matrix.txt", "r");
    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
            for(int k = 0; k < ACTIONS_ACC; k++)
                for(int m = 0; m < ACTIONS_DIRECTION; m++)
                    fscanf(fp, "%f", &Q[i][j][k][m]);
    
    fclose(fp);
    mutex.unlock();
}

void SaveLoad::initializeQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION) {

    FILE *fp = fopen("matrix.txt", "r");
    if(fp != NULL)
        readQ(chemin, Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
    else
        for(int i = 0; i < STATES_ALPHA; i++)
            for(int j = 0; j < STATES_DISTANCE; j++)
                for(int k = 0; k < ACTIONS_ACC; k++)
                    for(int m = 0; m < ACTIONS_DIRECTION; m++)
                        Q[i][j][k][m] = 0.;

}

}
