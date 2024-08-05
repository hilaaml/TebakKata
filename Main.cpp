#include <iostream>

using namespace std;

const int maxPlayer = 5;
int playerCount = 0, playerNum;

struct Player {
    string playerName;
    int playerScore = 0;
} P[maxPlayer];

struct Word {
    string word;
    string hint;
};

struct Node {
    Word data;
    Node* next = nullptr;
};


Node* head = nullptr;

Word wordList[] = {
                            {"kuda", "Mamalia berkuku ganjil dan oval, kepalanya memanjang, tengkuknya bersurai, ekornya panjang, serta biasa dipiara sebagai kendaraan (tunggangan, angkutan) atau penarik kendaraan."},
                            {"kucing", "Mamalia karnivor yang berukuran kecil hingga sedang, memiliki cakar berbentuk arit yang dapat keluar masuk dari kantong jari-jarinya, termasuk hewan teritorial."},
                            {"ular", "Reptilia, tidak berkaki, tubuhnya agak bulat memanjang, kulitnya bersisik, hidup di tanah atau di air, ada yang berbisa ada yang tidak."},
                            {"sapi", "Mamalia berkuku genap yang termasuk ke dalam kelompok ruminansia, bertubuh besar, bertanduk, berkaki empat, dipelihara untuk diambil daging dan susunya."},
                            {"ayam", "Unggas yang pada umumnya tidak dapat terbang, dapat dijinakkan dan dipelihara, berjengger, yang jantan berkokok dan bertaji, sedangkan yang betina berkotek dan tidak bertaji."}
};

const int wordCount = sizeof(wordList) / sizeof(wordList[0]);

void leaderboards(); // Sorting (insertion)
void playStart(Player& player); // Searching (mencocokkan percobaan tebakan dengan kata yang ditebak)
void playQueue(); // Queue (tambah)
void printQueue(); // Queue (tampil)
void addQueue(); // Struct, Array
bool playerNameSame(string temp); // Searching (mengecek nama player duplikat)
void addWordToList(const string& word, const string& hint); // Linked List (menyimpan kata tebakan)
void playerDel(); // Queue (menghapus antrian player)
void clearWordList(); // Linked List (clear)
void initializeWordList(); // Linkedlist (inisialisasi)

int main()
{
    int choices;
    int subChoices;

    initializeWordList();
    do {
        cout << "Permainan Tebak Kata\n";
        cout << "Menu:\n1. Mulai main\t\t\t3. Papan Skor \n2. Menu Antrian Pemain  \t0. Keluar\n\n";
        cout << "Masukkan pilihan = "; cin >> choices;
        system("cls");
        switch (choices) {
        case 0:
            clearWordList();
            cout << "Terima Kasih!";
            break;
        case 1:
            playQueue();
            break;
        case 2:
            do {
                printQueue();
                cout << "1. Tambah antrian\n2. Hapus antrian\n0. Kembali ke menu utama.\n\n";
                cout << "Masukkan pilihan = "; cin >> subChoices;
                system("cls");
                switch (subChoices) {
                case 0:
                    break;
                case 1:
                    addQueue();
                    break;
                case 2:
                    playerDel();
                    break;
                default:
                    cout << "Pilihan salah. Silahkan coba lagi.\n";
                    break;
                }
            } while (subChoices != 0);
            break;
        case 3:
            leaderboards();
            break;
        default:
            cout << "Pilihan salah.";
        }
        cout << "\n";
    } while (choices != 0);
    return 0;
}

bool playerNameSame(string temp) {
    for (int i = 0; i < playerCount; i++) {
        if (P[i].playerName == temp) {
            return true;
        }
    }
    return false;
}

void addQueue() {
    printQueue();
    cout << "Masukkan jumlah pemain yang ingin ditambahkan = "; cin >> playerNum;
    if (playerNum + playerCount <= maxPlayer or playerNum + playerCount <= wordCount) {
        for (int i = 0; i < playerNum; i++) {
            string temp;
            cout << "Masukkan nama pemain ke-" << (playerCount + 1) << " = "; cin >> temp;
            if (playerNameSame(temp)) {
                cout << "Nama pemain sudah ada. Silahkan masukkan nama lain." << endl;
                i--;
            }
            else {
                P[playerCount].playerName = temp;
                P[playerCount].playerScore = 0;
                playerCount++;
            }
        }
    }
    else {
        cout << "Maaf, jumlah antrian pemain sudah full (" << maxPlayer << ") atau melehihi jumlah kata yang tersedia (" << wordCount << ").\n";
    }
}

void leaderboards() {
    int i, j;
    Player temp;
    for (i = 1; i < playerCount; i++) {
        temp = P[i];
        j = i - 1;

        while ((j >= 0) && (P[j].playerScore < temp.playerScore)) {
            P[j + 1] = P[j];
            j--;
        }
        P[j + 1] = temp;
    }
    printQueue();
}

void playQueue() {
    if (playerCount == 0) {
        cout << "Permainan gagal dimulai.\n";
        addQueue();
    }
    else if (wordCount < playerCount) {
        cout << "Maaf jumlah player yang akan ditebak melebihi jumlah kata yang ditebak.\n";
    }
    else {
        for (int i = 0; i < playerCount; i++) {
            char playChoice;
            cout << "Giliran " << P[i].playerName << " bermain.\n";
            cout << "Apakah Anda ingin bermain? (y/n): "; cin >> playChoice;
            system("cls");
            if (playChoice == 'y' || playChoice == 'Y') {
                playStart(P[i]);
                cout << "Skor " << P[i].playerName << " adalah: " << P[i].playerScore << "\n";
            }
            else {
                cout << "Player " << P[i].playerName << " melewati giliran." << endl;
                cout << "Skor " << P[i].playerName << " adalah: " << P[i].playerScore << "\n";
            }
        }
    }
}


void displayStatus(const string& word, const string& guessed) {
    for (char characterToCheck : word) {
        if (guessed.find(characterToCheck) != string::npos) {
            cout << characterToCheck << " ";
        }
        else {
            cout << "_ ";
        }
    }
    cout << endl;
}

void playStart(Player& player) {
    string guess;
    bool validInput;

    static Node* currentNode = head;
    if (currentNode == nullptr) {
        cout << "Maaf, tidak ada lagi kata yang bisa ditebak.\n";
        return;
    }
    Word wordToGuess = currentNode->data;
    string guessedLetters = "";
    int attemptsLeft = wordToGuess.word.length() / 2;
    while (attemptsLeft > 0) {
        cout << "Hint: " << wordToGuess.hint << endl;
        displayStatus(wordToGuess.word, guessedLetters);
        cout << "Masukkan tebakan Anda: "; cin >> guess;
        system("cls");
        validInput = true;
        for (char characterToCheck : guess) {
            if (!isalpha(characterToCheck)) {
                validInput = false;
                break;
            }
        }
        if (!validInput) {
            cout << "Masukkan hanya huruf." << endl;
            continue;
        }
        if (guess.length() != 1 && guess.length() != wordToGuess.word.length()) {
            cout << "Masukkan satu huruf atau tebakan yang sesuai dengan panjang kata." << endl;
            continue;
        }
        if (guessedLetters.find(guess) != string::npos) {
            cout << "Anda sudah menebak huruf/kata ini sebelumnya." << endl;
            continue;
        }
        guessedLetters += guess;
        bool correctGuess = true;
        for (char characterToCheck : guess) {
            if (wordToGuess.word.find(characterToCheck) == string::npos) {
                correctGuess = false;
                break;
            }
        }
        if (correctGuess) {
            cout << "Tebakan Anda benar!" << endl;
        }
        else {
            cout << "Tebakan Anda salah!" << endl;
            attemptsLeft--;
            cout << "Kesempatan yang tersisa: " << attemptsLeft << endl;
        }
        bool allGuessed = true;
        for (char characterToCheck : wordToGuess.word) {
            if (guessedLetters.find(characterToCheck) == string::npos) {
                allGuessed = false;
                break;
            }
        }
        if (allGuessed) {
            cout << "Selamat! Anda berhasil menebak kata " << wordToGuess.word << ".\n";
            player.playerScore += 10;
            currentNode = currentNode->next;
            return;
        }
    }
    cout << "Kesempatan Anda habis! Kata yang harus ditebak adalah " << wordToGuess.word << ".\n";
    currentNode = currentNode->next;
}

void initializeWordList() {
    for (int i = 0; i < wordCount; i++) {
        addWordToList(wordList[i].word, wordList[i].hint);
    }
}

void addWordToList(const string& word, const string& hint) {
    Node* newNode = new Node;
    newNode->data.word = word;
    newNode->data.hint = hint;
    newNode->next = head;
    head = newNode;
}

void clearWordList() {
    Node* current = head;
    Node* nextNode;
    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
}

void printQueue() {
    cout << "Antrian Pemain\n";
    cout << "[!] Antrian akan berubah sesuai peringkat skor.\n";
    cout << "No.\tNama Pemain\t\tSkor\n";
    for (int i = 0; i < playerCount; i++) {
        cout << i + 1 << ".\t" << P[i].playerName << "\t\t" << P[i].playerScore << endl;
    }
    cout << "\n";
}

void playerDel() {
    printQueue();
    if (playerCount == 0) {
        cout << "Tidak ada pemain untuk dihapus.\n";
        return;
    }
    else {
        cout << "Masukkan nomor pemain yang ingin dihapus: "; cin >> playerNum;
        if (playerNum < 1 || playerNum > playerCount) {
            cout << "Nomor pemain tidak valid.\n";
            return;
        }
        else {
            int temp = playerNum - 1;
            for (int i = temp; i < playerCount - 1; i++) {
                P[i] = P[i + 1];
            }
            playerCount--;
            cout << "Pemain nomor " << playerNum << " berhasil dihapus.\n";
        }
    }
}
