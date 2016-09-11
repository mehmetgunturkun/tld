#include "detector/ensemble/EnsembleScore.hpp"

int main(int argc, char** args) {

    vector<float> scores1 = { 0.5f, 0.6f, 0.7f, 0.6f, 0.5f};
    EnsembleScore* ensembleScore1 = new EnsembleScore(scores1);

    vector<float> scores2 = { 0.3f, 0.2f, 0.1f, 0.2f, 0.3f};
    EnsembleScore* ensembleScore2 = new EnsembleScore(scores2);

    EnsembleScore* ensembleScore3 = (EnsembleScore*) ensembleScore1->merge(ensembleScore2);
    
    for (int i = 0; i < 5; i++) {
        float score1 = ensembleScore1->scores[i];
        float score2 = ensembleScore2->scores[i];
        float score3 = ensembleScore3->scores[i];

        printf("%d, %f, %f, %f\n",i, score1, score2, score3);
    }
    printf("Test is completed!\n");
}
