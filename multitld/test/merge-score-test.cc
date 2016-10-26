#include "detector/ensemble/EnsembleScore.hpp"

int main(int argc, char** args) {

    vector<double> scores1 = { 0.5, 0.6, 0.7, 0.6, 0.5};
    EnsembleScore* ensembleScore1 = new EnsembleScore(scores1);

    vector<double> scores2 = { 0.3, 0.2, 0.1, 0.2, 0.75};
    EnsembleScore* ensembleScore2 = new EnsembleScore(scores2);

    EnsembleScore* ensembleScore3 = (EnsembleScore*) ensembleScore1->sum(ensembleScore2);
    ensembleScore3 = (EnsembleScore*) ensembleScore3->divide(2);

    for (int i = 0; i < 5; i++) {
        double score1 = ensembleScore1->scores[i];
        double score2 = ensembleScore2->scores[i];
        double score3 = ensembleScore3->scores[i];

        printf("%d, %g, %g, %g\n",i, score1, score2, score3);
    }
    printf("Test is completed!\n");
}
