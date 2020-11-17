#include <stdio.h>
#include <string.h>

#define UNUSED(var) ((void)(var))

void switchCards(char *card, const int firstCardIndex, const int secondCardIndex)
{
    char helpChar = card[firstCardIndex];
    card[firstCardIndex] = card[secondCardIndex];
    card[secondCardIndex] = helpChar;

    helpChar = card[firstCardIndex + 1];
    card[firstCardIndex + 1] = card[secondCardIndex + 1];
    card[secondCardIndex + 1] = helpChar;
}

int getNumber(char number)
{
    switch (number) {
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'T':
            return 10;
        case 'J':
            return 11;
        case 'Q':
            return 12;
        case 'K':
            return 13;
        case 'A':
            return 14;
            // W is special card used for Ace to five combinations
        case 'W':
            return 1;
    }
    return number;
}

int isFirstGreaterThanSecond(const char first, const char second)
{
    int firstNumber = getNumber(first);
    int secondNumber = getNumber(second);
    return firstNumber > secondNumber;
}

// sorts from lowest rank to highest rank
void sortCards(char *cards)
{
    for (size_t i = 0; i < strlen(cards); i += 3) {
        for (size_t j = 3; j < strlen(cards); j += 3) {
            if (isFirstGreaterThanSecond(cards[j - 3], cards[j])) {
                switchCards(cards, j - 3, j);
            }
        }
    }
}

void sortCardsByRank(char *cards)
{
    for (size_t i = 0; i < strlen(cards); i += 3) {
        for (size_t j = 3; j < strlen(cards); j += 3) {
            if (!(isFirstGreaterThanSecond(cards[j - 3], cards[j]))) {
                switchCards(cards, j - 3, j);
            }
        }
    }
}

int isStraight(const char *cards, int *sum)
{
    int noInRow = 1;
    int sumOfCards = 0;
    char sortedCards[25];
    strncpy(sortedCards, cards, 22);
    sortCardsByRank(sortedCards);
    int arrayLength = 16;

    if (sortedCards[0] == 'A') { // fixes ace to five
        sortedCards[20] = ' ';
        sortedCards[21] = 'W';
        sortedCards[22] = 'h';
        sortedCards[23] = '\0';
        arrayLength = 19;
    }

    for (int i = 0; i < arrayLength; i += 3) {
        if (getNumber(sortedCards[i]) - getNumber(sortedCards[i + 3]) == 1) {
            noInRow++;
            sumOfCards += getNumber(sortedCards[i]);

            if (noInRow == 5) {
                *sum = sumOfCards + getNumber(sortedCards[i + 3]);
                return 5;
            }
            // if there is 2 same value cards in row
        } else if (i < 13 &&
                   getNumber(sortedCards[i]) - getNumber(sortedCards[i + 3]) == 0 &&
                   getNumber(sortedCards[i]) - getNumber(sortedCards[i + 6]) == 1) {
            noInRow++;
            sumOfCards += getNumber(sortedCards[i]);

            if (noInRow == 5) {
                *sum = sumOfCards + getNumber(sortedCards[i + 6]);
                return 5;
            }
            i += 3;
            // if there is 3 same value cards in row
        } else if (i < 10 &&
                   getNumber(sortedCards[i]) - getNumber(sortedCards[i + 6]) == 0 &&
                   getNumber(sortedCards[i]) - getNumber(sortedCards[i + 9]) == 1) {
            noInRow++;
            sumOfCards += getNumber(sortedCards[i]);
            if (noInRow == 5) {
                *sum = sumOfCards + getNumber(sortedCards[i + 9]);
                return 5;
            }
            i += 6;
        } else {
            noInRow = 1;
            sumOfCards = 0;
        }
    }

    return 0;
}

int isFlush(const char *cards, int *kickerOne, int *kickerTwo, int *kickerThree, int *kickerFour, int *kickerFive)
{
    int clubsCount = 0;
    int spadesCount = 0;
    int heartsCount = 0;
    int diamondsCount = 0;

    for (int i = 1; i < 20; i += 3) {
        switch (cards[i]) {
            case 'c':
                ++clubsCount;
                break;
            case 's':
                ++spadesCount;
                break;
            case 'h':
                ++heartsCount;
                break;
            case 'd':
                ++diamondsCount;
                break;
        }
        if (i > 12) { // after 5 checked cards
            if (clubsCount >= 5 || spadesCount >= 5 || heartsCount >= 5 ||
                diamondsCount >= 5) {
                char symbol;
                int kicker1 = 0;
                int kicker2 = 0;
                int kicker3 = 0;
                int kicker4 = 0;
                int kicker5 = 0;

                if (clubsCount > 4) {
                    symbol = 'c';
                } else if (spadesCount > 4) {
                    symbol = 's';
                } else if (heartsCount > 4) {
                    symbol = 'h';
                } else
                    symbol = 'd';

                for (int i = 0; i < 20; i += 3) {
                    if (cards[i + 1] == symbol && getNumber(cards[i]) > kicker1) {
                        kicker5 = kicker4;
                        kicker4 = kicker3;
                        kicker3 = kicker2;
                        kicker2 = kicker1;
                        kicker1 = getNumber(cards[i]);
                    }
                }
                *kickerOne = kicker1;
                *kickerTwo = kicker2;
                *kickerThree = kicker3;
                *kickerFour = kicker4;
                *kickerFive = kicker5;

                return 4;
            }
        }
    }
    return 0;
}

// returns 2 if true, value of four of kind in kicker1 and highest kicker in
// kicker2
int isFourOfKind(const char *cards, int *kicker1, int *kicker2)
{
    int noInRow = 1;
    int isFourOfKind = 0;
    int i = 0;
    int rowNumber = 0;
    int kickerCard = 0;

    while (!isFourOfKind && i < 16) {
        if (getNumber(cards[i]) - getNumber(cards[i + 3]) == 0) {
            noInRow++;

            if (noInRow == 4) {
                rowNumber = getNumber(cards[i]);
                isFourOfKind = 1;
            }
        } else {
            noInRow = 1;
        }
        i += 3;
    }

    if (isFourOfKind) {
        for (int j = 0; j < 16; j += 3) {
            if (getNumber(cards[j]) != rowNumber) {
                if (getNumber(cards[j]) > kickerCard) {
                    kickerCard = getNumber(cards[j]);
                }
            }
        }
        *kicker1 = rowNumber;
        *kicker2 = kickerCard;
        return 2;
    }
    return 0;
}

int isTreeOfKind(const char *cards, int *threeOfkindValue, int *highKicker, int *lowKicker)
{
    int noInRow = 1;
    int rowNumber = -1;
    int firstMaxCard = 0;
    int secondMaxCard = 0;
    int isThreeOfKind = 0;
    int i = 0;

    while (!isThreeOfKind && i < 16) {
        if (getNumber(cards[i]) - getNumber(cards[i + 3]) == 0) {
            noInRow++;

            if (noInRow == 3) {
                rowNumber = getNumber(cards[i]);
                isThreeOfKind = 1;
            }
        } else {
            noInRow = 1;
        }
        i += 3;
    }
    // Gets kickers
    if (isThreeOfKind) {
        for (int j = 0; j < 16; j += 3) {
            if (getNumber(cards[j]) != rowNumber) {
                if (getNumber(cards[j]) > firstMaxCard) {
                    secondMaxCard = firstMaxCard;
                    firstMaxCard = getNumber(cards[j]);
                }
            }
        }
        *threeOfkindValue = rowNumber;
        *highKicker = firstMaxCard;
        *lowKicker = secondMaxCard;

        return 6;
    }
    return 0;
}

int isTwoPair(const char *cards, int *kicker1, int *kicker2, int *kicker3)
{
    int cardRepeats = 1;
    int noOfCardRepeats = 0;
    int firstPair = 0;
    int secondPair = 0;
    int isTwoPair = 0;
    int maxCard = 0;
    int i = 0;
    int k = i + 3;

    while (i < 16 && !isTwoPair) {
        cardRepeats = 1;
        while (k < 19 && !isTwoPair) {
            if (getNumber(cards[i]) == getNumber(cards[k])) {
                cardRepeats++;
                if (cardRepeats == 2) {
                    cardRepeats = 1;

                    if (noOfCardRepeats == 0) {
                        firstPair = getNumber(cards[i]);
                    } else
                        secondPair = getNumber(cards[i]);
                    noOfCardRepeats++;

                    if (noOfCardRepeats == 2) {
                        isTwoPair = 1; // is True
                    }
                }
            }
            k += 3;
        }
        i += 3;
        k = i + 3;
    }
    if (isTwoPair) {
        // firstPair muset be bigger than secondPair for draw case of game
        if (getNumber(firstPair) < getNumber(secondPair)) {
            int temp = firstPair;
            firstPair = secondPair;
            secondPair = temp;
        }

        for (int j = 0; j < 19; j += 3) {
            if (getNumber(cards[j]) != getNumber(firstPair) ||
                getNumber(cards[j]) != getNumber(secondPair)) {
                if (getNumber(cards[j]) > maxCard) {
                    maxCard = getNumber(cards[j]);
                }
            }
        }
        *kicker1 = firstPair;
        *kicker2 = secondPair;
        *kicker3 = maxCard;
        return 7;
    }
    return 0;
}

int isPair(const char *cards, int *pairValue, int *kicker1, int *kicker2, int *kicker3)
{
    int isPair = 0;
    int i = 0;
    int firstMaxCard = 0;
    int secondMaxCard = 0;
    int thirdMaxCard = 0;

    while (!isPair && i < 16) {
        if (getNumber(cards[i]) - getNumber(cards[i + 3]) == 0) {
            isPair = getNumber(cards[i]);
        }
        i += 3;
    }
    if (isPair) {
        for (int j = 0; j < 16; j += 3) {
            if (getNumber(cards[j]) != isPair) {
                if (getNumber(cards[j]) > firstMaxCard) {
                    thirdMaxCard = secondMaxCard;
                    secondMaxCard = firstMaxCard;
                    firstMaxCard = getNumber(cards[j]);
                }
            }
        }
        *pairValue = isPair;
        *kicker1 = firstMaxCard;
        *kicker2 = secondMaxCard;
        *kicker3 = thirdMaxCard;
        return 8;
    }
    return 0;
}

// returns 3 if true, tree of kind in kicker1, two of kind in kicker2, 0
// otherwise
int isFullHouse(const char *cards, int *kicker1, int *kicker2)
{
    int noOfRepeats = 1;
    int isThreeOfAKind = 0;
    int isTwoOfAKind = 0;
    // find trio;
    for (int i = 0; i < 16; i += 3) {
        if (getNumber(cards[i]) == getNumber(cards[i + 3])) {
            noOfRepeats++;
            if (noOfRepeats == 3) {
                isThreeOfAKind = getNumber(cards[i]);
                noOfRepeats = 1;
            }
        } else {
            noOfRepeats = 1;
        }
    }
    // find duo
    if (isThreeOfAKind) {
        for (int i = 0; i < 16; i += 3) {
            if (getNumber(cards[i]) != isThreeOfAKind &&
                getNumber(cards[i]) == getNumber(cards[i + 3])) {
                noOfRepeats++;
                if (noOfRepeats == 2) {
                    isTwoOfAKind = getNumber(cards[i]);
                    *kicker1 = isThreeOfAKind;
                    *kicker2 = isTwoOfAKind;
                    return 3;
                }
            } else {
                noOfRepeats = 1;
            }
        }
    }
    return 0;
}

// returns 1 if true, sum of straight cards to sum, 0 otherwise
int isStraightFlush(const char *cards, int *sum)
{
    int sum1 = 0;
    int kicker1 = 0;
    int kicker2 = 0;
    int kicker3 = 0;
    int kicker4 = 0;
    int kicker5 = 0;
    if (isStraight(cards, &sum1)) {
        if (isFlush(cards, &kicker1, &kicker2, &kicker3, &kicker4, &kicker5)) {
            if (kicker1 + kicker2 + kicker3 + kicker4 + kicker5 == sum1) {
                *sum = sum1;
                return 1;
            }
        }
    }
    return 0;
}

int isHighCard(const char *cards, int *highestCard)
{
    int max = 0;
    for (int i = 0; i < 19; i += 3) {
        if (getNumber(cards[i]) > max) {
            max = getNumber(cards[i]);
        }
    }
    *highestCard = max;
    return 9;
}

// finds best combination of players cards
int getCombination(const char *firstWithRiver, int *firstKicker1, int *firstKicker2, int *firstKicker3, int *firstKicker4, int *firstKicker5, int *sum1)
{
    int i = 1;
    int result = 0;
    while (result == 0) {
        switch (i) {
            case 1:
                result = isStraightFlush(firstWithRiver, sum1);
                break;
            case 2:
                result = isFourOfKind(firstWithRiver, firstKicker1, firstKicker2);
                break;
            case 3:
                result = isFullHouse(firstWithRiver, firstKicker1, firstKicker2);
                break;
            case 4:
                result = isFlush(firstWithRiver, firstKicker1, firstKicker2, firstKicker3, firstKicker4, firstKicker5);
                break;
            case 5:
                result = isStraight(firstWithRiver, sum1);
                break;
            case 6:
                result = isTreeOfKind(firstWithRiver, firstKicker1, firstKicker2, firstKicker3);
                break;
            case 7:
                result =
                        isTwoPair(firstWithRiver, firstKicker1, firstKicker2, firstKicker3);
                break;
            case 8:
                result = isPair(firstWithRiver, firstKicker1, firstKicker2, firstKicker3, firstKicker3);
                break;
            default:
                result = isHighCard(firstWithRiver, firstKicker1);
                break;
        }
        i++;
    }
    return result;
}

char getResult(const int *first, const int *second)
{
    if (*first > *second) {
        return 'W';
    }
    if (*first < *second) {
        return 'L';
    }
    if (*first == *second) {
        return 'D';
    }
    return 0;
}

int main(int argc, char *argv[])
{
    /**************************************************/
    /* ------------- IMPORTANT NOTICE --------------- */
    /* Before submitting your homework, make sure you */
    /* REMOVE the following two lines with UNUSED:    */
    /**************************************************/
    UNUSED(argc);
    UNUSED(argv);
    // Init first player
    int firstSum = 0;
    int firstKicker1 = 0;
    int firstKicker2 = 0;
    int firstKicker3 = 0;
    int firstKicker4 = 0;
    int firstKicker5 = 0;
    char firstHand[6];
    char firstWithRiver[21];

    // Init second player
    int secondSum = 0;
    int secondKicker1 = 0;
    int secondKicker2 = 0;
    int secondKicker3 = 0;
    int secondKicker4 = 0;
    int secondKicker5 = 0;
    char secondHand[6];
    char secondWithRiver[21];

    char riverHand[50];
    char output[4000];
    int outputLength = 0;

    //Dlouho jsem si lamal hlavu nad tím, jak skončit while jinak, než zmáčknutím enteru, nepřišel jsem na to.
    while (scanf("%[^\n]%*c", firstHand) != 0) {
        scanf("%[^\n]%*c", secondHand);
        scanf("%[^\n]%*c", riverHand);
        // merge first player hand with river

        strncpy(firstWithRiver, firstHand, 6);
        strncat(firstWithRiver, " ", 2);
        strncat(firstWithRiver, riverHand, 21);

        // merge second player hand with river
        strncpy(secondWithRiver, secondHand, 6);
        strncat(secondWithRiver, " ", 2);
        strncat(secondWithRiver, riverHand, 21);

        // sorts cards from lowest to highest value
        sortCards(firstWithRiver);
        sortCards(secondWithRiver);

        int firstResult =
                getCombination(firstWithRiver, &firstKicker1, &firstKicker2, &firstKicker3, &firstKicker4, &firstKicker5, &firstSum);
        int secondResult = getCombination(
                secondWithRiver, &secondKicker1, &secondKicker2, &secondKicker3, &secondKicker4, &secondKicker5, &secondSum);

        char res;
        // lower result is better
        if (firstResult < secondResult) {
            output[outputLength] = 'W';
            outputLength++;
        } else if (firstResult > secondResult) {
            output[outputLength] = 'L';
            outputLength++;
        } else if (firstResult == 1) { // straight flush
            output[outputLength] = getResult(&firstSum, &secondSum);
            outputLength++;
        } else if (firstResult == 2) {
            res = getResult(&firstKicker1, &secondKicker1);
            if (res != 'D') {
                output[outputLength] = res;
                outputLength++;
            } else {
                output[outputLength] = getResult(&firstKicker2, &secondKicker2);
                outputLength++;
            }
        } else if (firstResult == 3) { // full house
            res = getResult(&firstKicker1, &secondKicker1);
            if (res != 'D') {
                output[outputLength] = res;
                outputLength++;
            } else {
                output[outputLength] = getResult(&firstKicker2, &secondKicker2);
                outputLength++;
            }
        } else if (firstResult == 4) {
            res = getResult(&firstKicker1, &secondKicker1);
            if (res != 'D') {
                output[outputLength] = res;
                outputLength++;
            } else {
                res = getResult(&firstKicker2, &secondKicker2);
                if (res != 'D') {
                    output[outputLength] = res;
                    outputLength++;
                } else {
                    res = getResult(&firstKicker3, &secondKicker3);
                    if (res != 'D') {
                        output[outputLength] = res;
                        outputLength++;
                    } else {
                        res = getResult(&firstKicker4, &secondKicker4);
                        if (res != 'D') {
                            output[outputLength] = res;
                            outputLength++;
                        } else {
                            res = getResult(&firstKicker5, &secondKicker5);
                            output[outputLength] = res;
                            outputLength++;
                        }
                    }
                }
            }
        } else if (firstResult == 5) {
            output[outputLength] = getResult(&firstSum, &secondSum);
            outputLength++;
        } else if (firstResult == 6) {
            res = getResult(&firstKicker1, &secondKicker1);
            if (res != 'D') {
                output[outputLength] = res;
                outputLength++;
            } else {
                res = getResult(&firstKicker2, &secondKicker2);
                if (res != 'D') {
                    output[outputLength] = res;
                    outputLength++;
                } else {
                    res = getResult(&firstKicker3, &secondKicker3);
                    output[outputLength] = res;
                    outputLength++;
                }
            }
        } else if (firstResult == 7) {
            res = getResult(&firstKicker1, &secondKicker1);
            if (res != 'D') {
                output[outputLength] = res;
                outputLength++;
            } else {
                res = getResult(&firstKicker2, &secondKicker2);
                if (res != 'D') {
                    output[outputLength] = res;
                    outputLength++;
                } else {
                    res = getResult(&firstKicker3, &secondKicker3);
                    output[outputLength] = res;
                    outputLength++;
                }
            }
        } else if (firstResult == 8) {
            res = getResult(&firstKicker1, &secondKicker1);
            if (res != 'D') {
                output[outputLength] = res;
                outputLength++;
            } else {
                res = getResult(&firstKicker2, &secondKicker2);
                if (res != 'D') {
                    output[outputLength] = res;
                    outputLength++;
                } else {
                    res = getResult(&firstKicker3, &secondKicker3);
                    if (res != 'D') {
                        output[outputLength] = res;
                        outputLength++;
                    } else {
                        res = getResult(&firstKicker4, &secondKicker4);
                        output[outputLength] = res;
                        outputLength++;
                    }
                }
            }
        } else if (firstResult == 9) {
            res = getResult(&firstKicker1, &secondKicker1);
            output[outputLength] = res;
            outputLength++;
        }
    }

    // Print results
    for (int i = 0; i < outputLength; ++i) {
        printf("%c\n", output[i]);
    }

    return 0;
}