#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>

using namespace std;

#define HIST_WIDTH 320
#define HIST_HEIGHT 200
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 9
#define DIB_HEADER_SIZE 40
#define BMP_HEADER_SIZE 14
#define LEFT_PADDING 13
#define CHART_HEIGHT 150
#define CHART_BOTTOM_PADDING 30
#define TEXT_BOTTOM_PADDING 5
#define NUMBER_COUNT 49
#define NUMBERS_PER_LOTTERY 6
#define HOT_N_NOT_COUNT 6

char zeroChar[10][CHAR_HEIGHT][CHAR_WIDTH + 1] =
{
  {"001100",///0
  "010010",
  "100001",
  "100001",
  "100001",
  "100001",
  "100001",
  "010010",
  "001100"},
  {"000100",///1
  "001100",
  "010100",
  "000100",
  "000100",
  "000100",
  "000100",
  "000100",
  "011110"},
  {"011110",///2
  "100001",
  "000001",
  "000010",
  "000100",
  "001000",
  "010000",
  "100000",
  "111111"},
  {"011110",///3
  "100001",
  "000001",
  "000001",
  "000110",
  "000001",
  "000001",
  "100001",
  "011110"},
  {"000110",///4
  "000110",
  "001010",
  "010010",
  "100010",
  "100010",
  "111110",
  "000010",
  "000010"},
  {"111111",///5
  "100000",
  "100000",
  "100000",
  "111110",
  "000001",
  "000001",
  "100001",
  "011110"},
  {"011110",///6
  "100001",
  "100000",
  "100000",
  "011110",
  "100001",
  "100001",
  "100001",
  "011110"},
  {"111111",///7
  "110011",
  "100001",
  "000010",
  "000010",
  "000100",
  "000100",
  "001000",
  "001000"},
  {"011110",///8
  "100001",
  "100001",
  "100001",
  "011110",
  "100001",
  "100001",
  "100001",
  "011110"},
  {"011110",///9
  "100001",
  "100001",
  "100001",
  "011110",
  "000001",
  "000001",
  "100001",
  "011110"}
};

struct headerDIB {
    char data[DIB_HEADER_SIZE];

    headerDIB* bSize(int i) {
        putIntOffet(i, 0);
        return this;
    }
    headerDIB* bWidth(int i) {
        putIntOffet(i, 4);
        return this;
    }
    headerDIB* bHeight(int i) {
        putIntOffet(i, 8);
        return this;
    }
    headerDIB* bPlanes(short s) {
        putShortOffet(s, 12);
        return this;
    }
    headerDIB* bBitCount(short s) {
        putShortOffet(s, 14);
        return this;
    }
    headerDIB* bCompression(int i) {
        putIntOffet(i, 16);
        return this;
    }
    headerDIB* bSizeImage(int i) {
        putIntOffet(i, 20);
        return this;
    }
    headerDIB* bXPelsPerMeter(int i) {
        putIntOffet(i, 24);
        return this;
    }
    headerDIB* bYPelsPerMeter(int i) {
        putIntOffet(i, 28);
        return this;
    }
    headerDIB* bClrUsed(int i) {
        putIntOffet(i, 32);
        return this;
    }
    headerDIB* biClrImportant(char c) {
        data[37] = c;
        return this;
    }
    headerDIB* biClrRotation(char c) {
        data[37] = c;
        return this;
    }
    headerDIB* biReserved(short s) {
        putShortOffet(s, 38);
        return this;
    }

    void putIntOffet(int i, int offset) {
        int* pointer;
        pointer = (int*)&data[offset];
        (*pointer) = i;
    }
    void putShortOffet(short s, int offset) {
        short* pointer;
        pointer = (short*)&data[offset];
        (*pointer) = s;
    }
};

struct headerBMP {
    char data[BMP_HEADER_SIZE];

    headerBMP* bm(char a, char b) {
        data[0] = a;
        data[1] = b;
        return this;
    }
    headerBMP* length(int length) {
        int* i;
        i = (int*)&data[2];
        (*i) = length;
        return this;
    }
    headerBMP* xy(short x, short y) {
        short* s;
        s = (short*)&data[6];
        (*s) = x;
        s++;
        (*s) = y;
        return this;
    }
    headerBMP* offset(int offset) {
        int* i;
        i = (int*)&data[10];
        (*i) = offset;
        return this;
    }
};

struct pixel {
    char B;
    char G;
    char R;
};

struct BMP {
    headerBMP hbmp;
    headerDIB hdib;
    pixel pixels[HIST_HEIGHT][HIST_WIDTH];

    BMP() {
        hbmp.length(3 * HIST_HEIGHT * HIST_WIDTH + DIB_HEADER_SIZE + BMP_HEADER_SIZE)->bm('B', 'M')->offset(DIB_HEADER_SIZE + BMP_HEADER_SIZE)->xy(0, 0);
        hdib.bSize(DIB_HEADER_SIZE)->bWidth(HIST_WIDTH)->bHeight(HIST_HEIGHT)->bPlanes(1)->bBitCount(24)->bCompression(0);
        hdib.bSizeImage(3 * HIST_HEIGHT * HIST_WIDTH)->bXPelsPerMeter(0)->bYPelsPerMeter(0)->biClrImportant(0);
        hdib.biClrRotation(0)->biReserved(0);
        for (int i = 0; i < HIST_WIDTH; i++) {
            for (int j = 0; j < HIST_HEIGHT; j++) {
                pixels[j][i].B = (i * 255) / HIST_WIDTH;
                pixels[j][i].R = (j * 255) / HIST_HEIGHT;
                pixels[j][i].G = 0;
            }

        }
        for (int i = 1; i <= NUMBER_COUNT; i++) {
            if (i > 9)
                putX(TEXT_BOTTOM_PADDING + CHAR_WIDTH + 1, LEFT_PADDING + i * CHAR_WIDTH, i / 10);
            putX((i < 10 ? CHAR_WIDTH + 1 : TEXT_BOTTOM_PADDING), LEFT_PADDING + i * CHAR_WIDTH, i % 10);
        }
    }
    void putX(int x, int y, int n) {
        for (int i = 0; i < CHAR_WIDTH; i++) {
            for (int j = 0; j < CHAR_HEIGHT; j++) {
                pixels[x + j][y + i].R = 255 * (zeroChar[n][CHAR_HEIGHT - j - 1][i] - '0');
                pixels[x + j][y + i].G = 255 * (zeroChar[n][CHAR_HEIGHT - j - 1][i] - '0');
                pixels[x + j][y + i].B = 255 * (zeroChar[n][CHAR_HEIGHT - j - 1][i] - '0');
            }
        }
    }

    void drawColumn(int number, int height) {
        for (int i = CHART_BOTTOM_PADDING; i < CHART_BOTTOM_PADDING + CHART_HEIGHT; i++) {
            if (i > height + CHART_BOTTOM_PADDING)
                break;
            char greenIntensivity[CHAR_WIDTH] = { 20,120,210,255,150,80 };
            for (int j = 0; j < CHAR_WIDTH; j++) {
                pixels[i][LEFT_PADDING + CHAR_WIDTH * (number)+j].G = greenIntensivity[j];
            }
        }
    }
};

void randomGenerator(int n) {
    fstream output;
    output.open("datafile.txt", ios::out);
    srand(time(NULL));
    for (int i = 1; i < n + 1; i++) {
        output << i << ". " << (1 + (rand() % 29)) << "." << (1 + (rand() % 12)) << "." << (1900 + (rand() % 115)) << " ";
        output << 1 + rand() % NUMBER_COUNT << ",";
        output << 1 + rand() % NUMBER_COUNT << ",";
        output << 1 + rand() % NUMBER_COUNT << ",";
        output << 1 + rand() % NUMBER_COUNT << ",";
        output << 1 + rand() % NUMBER_COUNT << ",";
        output << 1 + rand() % NUMBER_COUNT << endl;
    }
    output.close();
}

struct Date {
    int day;
    int month;
    int year;
    bool operator ==(const Date& d) {
        if ((this->day == d.day) && (this->month == d.month) && (this->year == d.year))
            return true;
        else
            return false;
    }
    bool operator <(const Date& d) {
        if (d.year > this->year)
            return true;
        if (d.year < this->year)
            return false;
        if (d.month > this->month)
            return true;
        if (d.month < this->month)
            return false;
        if (d.day > this->day)
            return true;
        return false;
    }
    bool operator >(const Date& d) {
        if (d.year < this->year)
            return true;
        if (d.year > this->year)
            return false;
        if (d.month < this->month)
            return true;
        if (d.month > this->month)
            return false;
        if (d.day < this->day)
            return true;
        return false;
    }
    bool isCorrect() {
        if (day < 1 || day>31)
            return false;
        if (month < 1 || month>12)
            return false;
        if (year < 1)
            return false;
        return true;
    }
};

struct Line {
    int lp;
    Date date;
    int numbers[NUMBERS_PER_LOTTERY];
};

bool isNumber(char c) {
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

fstream* getLogger() {
    static fstream logger;
    if (!logger.is_open()) {
        logger.open("log.txt", ios::out);
        if (!logger.is_open()) {
            cerr << "Critital error while creating logger!!!!" << endl;
        }
    }//*/
    return &logger;
}

Line readLine(fstream& plik) {
    Line line;
    char buff[256];

    if (plik.eof())
        return line;
    plik.getline(buff, 256);
    if (plik.eof())
        return line;
    int pos = 0;
    int c = -1;
    char tmp[16][16];
    int posx = 0;
    while (buff[pos] != '\0') {
        if (isNumber(buff[pos]) && posx > 0) {
            tmp[c][posx++] = buff[pos];
        }
        if (isNumber(buff[pos]) && posx == 0) { ///czy pocz1tek liczby
            tmp[++c][posx++] = buff[pos];
        }
        if (!isNumber(buff[pos]) && posx > 0) {
            tmp[c][posx++] = '\0';
            posx = 0;
        }
        pos++;
    }
    tmp[c][posx++] = '\0';

    int results[10];
    for (int i = 0; i < 10; i++) {
        int x = 0;
        int bff = 0;
        while (tmp[i][x] != '\0') {
            bff *= 10;
            bff += tmp[i][x++] - '0';
        }
        results[i] = bff;
    }
    line.lp = results[0];
    line.date.day = results[1];
    line.date.month = results[2];
    line.date.year = results[3];
    for (int i = 0; i < NUMBERS_PER_LOTTERY; i++) {
        line.numbers[i] = results[i + 4];
    }
    return line;
}

Date readDate() {
    Date d;
    char buff[256];
    cin.sync();
    cin.clear();
    cin.getline(buff, 256);
    int pos = 0;
    int c = -1;
    char tmp[4][16];
    int posx = 0;
    while (buff[pos] != '\0') {
        if (isNumber(buff[pos]) && posx > 0) {
            tmp[c][posx++] = buff[pos];
        }
        if (isNumber(buff[pos]) && posx == 0) {
            tmp[++c][posx++] = buff[pos];
        }
        if (!isNumber(buff[pos]) && posx > 0) {
            tmp[c][posx++] = '\0';
            posx = 0;
        }
        pos++;
    }
    tmp[c][posx++] = '\0';

    int results[3];
    for (int i = 0; i < 3; i++) {
        int x = 0;
        int bff = 0;
        while (tmp[i][x] != '\0') {
            bff *= 10;
            bff += tmp[i][x++] - '0';
        }
        results[i] = bff;
    }
    d.day = results[0];
    d.month = results[1];
    d.year = results[2];
    return d;
}

void printHotNNot() {
    cout << "_  _ ____ ___ . _  _ . _  _ ____ ___ " << endl;
    cout << "|__| |  |  |  ' |\\ | ' |\\ | |  |  |  " << endl;
    cout << "|  | |__|  |    | \\|   | \\| |__|  |  " << endl;
    cout << "                                     " << endl;
}

void printProbabilities() {
    cout << "___  ____ ____ ___  ____ ___  _ _    _ ___ _ ____ ____ " << endl;
    cout << "|__] |__/ |  | |__] |__| |__] | |    |  |  | |___ [__  " << endl;
    cout << "|    |  \\ |__| |__] |  | |__] | |___ |  |  | |___ ___] " << endl;
    cout << "                                                       " << endl;
}

void sortHistogram(int in[NUMBER_COUNT]) {
    int sorted[NUMBER_COUNT];
    int mapping[NUMBER_COUNT];
    int sum = 0;
    for (int i = 0; i < NUMBER_COUNT; i++) {
        mapping[i] = i + 1;
        sorted[i] = in[i];
        sum += sorted[i];
    }

    for (int i = 0; i < NUMBER_COUNT - 1; i++) {
        for (int j = 0; j < NUMBER_COUNT - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int x = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = x;
                x = mapping[j];
                mapping[j] = mapping[j + 1];
                mapping[j + 1] = x;
            }
        }
    }

    printHotNNot();

    cout << "\tHOT\t|NOT" << endl;
    (*getLogger()) << "\tHOT\t|NOT" << endl;
    cout << "\t--------+--------" << endl;
    (*getLogger()) << "\t--------+--------" << endl;
    for (int i = 0; i < HOT_N_NOT_COUNT; i++) {
        cout << "\t" << i + 1 << ":" << mapping[NUMBER_COUNT - 1 - i] << "\t|" << i + 1 << ":" << mapping[i] << endl;
        (*getLogger()) << "\t" << i + 1 << ":" << mapping[NUMBER_COUNT - 1 - i] << "\t|" << i + 1 << ":" << mapping[i] << endl;
    }
}

void loadInput(Date start, Date stop, int fired[NUMBER_COUNT], fstream& input) {
    int lppredicted = 1;
    while (!input.eof()) {
        Line l = readLine(input);
        if (input.eof())
            break;
        if (l.lp != lppredicted) {
            cerr << "lppredicted: " << lppredicted << " read: " << l.lp << endl;
            break;
        }
        lppredicted++;
        if (l.date<start || l.date>stop)
            continue;

        for (int i = 0; i < NUMBERS_PER_LOTTERY; i++) {
            fired[l.numbers[i] - 1]++;
        }
    }
}

void findProbabilities(int fired[NUMBER_COUNT]) {
    printProbabilities();
    (*getLogger()) << endl << "Probabilities:" << endl;
    int sum = 0;
    for (int i = 0; i < NUMBER_COUNT; i++) {
        sum += fired[i];
    }
    for (int i = 0; i < NUMBER_COUNT; i++) {
        cout << "\t" << i + 1 << ":\t" << 100 * (double)fired[i] / (double)sum << "%" << endl;
        (*getLogger()) << "\t" << i + 1 << ":\t" << 100 * (double)fired[i] / (double)sum << "%" << endl;
    }
}

void makeHistogram(int fired[NUMBER_COUNT]) {
    int maximum = 0;
    for (int i = 0; i < NUMBER_COUNT; i++) {
        if (fired[i] > maximum) {
            maximum = fired[i];
        }
    }

    BMP bmp;
    for (int i = 0; i < NUMBER_COUNT; i++) {
        bmp.drawColumn(i + 1, (fired[i] * CHART_HEIGHT) / maximum);
    }

    fstream plik;
    plik.open("wynik.bmp", ios::binary | ios::out);
    plik.write((char*)&(bmp.hbmp.data), BMP_HEADER_SIZE);
    plik.write((char*)&(bmp.hdib.data), DIB_HEADER_SIZE);
    plik.write((char*)&(bmp.pixels), 3 * HIST_HEIGHT * HIST_WIDTH);
    plik.close();
}

void printFooter() {
    cout << "                                                                   " << endl;
    cout << "_   _ ____ _  _ . ____ ____    _ _ _ ____ _    ____ ____ _  _ ____ " << endl;
    cout << " \\_/  |  | |  | ' |__/ |___    | | | |___ |    |    |  | |\\/| |___ " << endl;
    cout << "  |   |__| |__|   |  \\ |___    |_|_| |___ |___ |___ |__| |  | |___ " << endl;
    cout << "                                                                   " << endl;
}

int main()
{
    randomGenerator(300); ///ficzer, pozwala wygenerowac dane wejsciowe, zapisuje do datafile.txt

    string inputname;
    fstream input;
    Date start, stop;
    int fired[NUMBER_COUNT];
    memset(fired, 0, sizeof(int) * NUMBER_COUNT);

    cout << "Podaj plik z danymi: ";
    cin >> inputname;
    input.open(inputname.c_str(), ios::in);
    if (!input.is_open()) {
        cerr << "Blad otwierania pliku!!!!" << endl;
        return -1;
    }

    cout << "Podaj poczatek okresu(DD.MM.RRRR): ";
    start = readDate();
    if (!start.isCorrect()) {
        cerr << "Niepoprawna data!!!" << endl;
        return-1;
    }
    cout << "Wybrany zostal: " << start.day << "." << start.month << "." << start.year << endl;
    cout << "Podaj koniec okresu(DD.MM.RRRR): ";
    stop = readDate();
    if (!stop.isCorrect()) {
        cerr << "Niepoprawna data!!!" << endl;
        return-1;
    }
    cout << "Wybrany zostal: " << stop.day << "." << stop.month << "." << stop.year << endl;
    if (start > stop) {
        cerr << "Blad: koniec okresu mniejszy od poczatku!!!!111oneoneone" << endl;
        return -1;
    }

    loadInput(start, stop, fired, input);
    sortHistogram(fired);
    findProbabilities(fired);
    makeHistogram(fired);

    (*getLogger()).close();
    printFooter();
    return 0;
}
