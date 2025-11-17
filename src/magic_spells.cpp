/*
 * From Hackerrank - C++ Domain - Dynamic Casting and inheritance.
 */
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Spell { 
    private:
        string scrollName;
    public:
        Spell(): scrollName("") { }
        Spell(string name): scrollName(name) { }
        virtual ~Spell() { }
        string revealScrollName() {
            return scrollName;
        }
};

class Fireball : public Spell { 
    private: int power;
    public:
        Fireball(int power): power(power) { }
        void revealFirepower(){
            cout << "Fireball: " << power << endl;
        }
};

class Frostbite : public Spell {
    private: int power;
    public:
        Frostbite(int power): power(power) { }
        void revealFrostpower(){
            cout << "Frostbite: " << power << endl;
        }
};

class Thunderstorm : public Spell { 
    private: int power;
    public:
        Thunderstorm(int power): power(power) { }
        void revealThunderpower(){
            cout << "Thunderstorm: " << power << endl;
        }
};

class Waterbolt : public Spell { 
    private: int power;
    public:
        Waterbolt(int power): power(power) { }
        void revealWaterpower(){
            cout << "Waterbolt: " << power << endl;
        }
};

class SpellJournal {
    public:
        static string journal;
        static string read() {
            return journal;
        }
}; 
string SpellJournal::journal = "";

void counterspell(Spell *spell) {
// Try to identify the four known spells with dynamic_cast
    if (Fireball* fb = dynamic_cast<Fireball*>(spell)) {
        fb->revealFirepower();
        return;
    }
    if (Frostbite* fr = dynamic_cast<Frostbite*>(spell)) {
        fr->revealFrostpower();
        return;
    }
    if (Waterbolt* wb = dynamic_cast<Waterbolt*>(spell)) {
        wb->revealWaterpower();
        return;
    }
    if (Thunderstorm* ts = dynamic_cast<Thunderstorm*>(spell)) {
        ts->revealThunderpower();
        return;
    }

    // Generic scroll spell -> compute LCS length with the journal
    string scroll  = spell->revealScrollName();     // e.g. "AquaVitae"
    string journal = SpellJournal::read();          // e.g. "AruTaVae"

    size_t m = scroll.size();
    size_t n = journal.size();

    // Space-optimized DP (two rows only)
    vector<int> prev(n + 1, 0);
    vector<int> curr(n + 1, 0);

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (scroll[i - 1] == journal[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        prev.swap(curr);
    }

    cout << prev[n] << '\n';
}

class Wizard {
    public:
        Spell *cast() {
            Spell *spell;
            string s; cin >> s;
            int power; cin >> power;
            if(s == "fire") {
                spell = new Fireball(power);
            }
            else if(s == "frost") {
                spell = new Frostbite(power);
            }
            else if(s == "water") {
                spell = new Waterbolt(power);
            }
            else if(s == "thunder") {
                spell = new Thunderstorm(power);
            } 
            else {
                spell = new Spell(s);
                cin >> SpellJournal::journal;
            }
            return spell;
        }
};

int main() {
    cout << "Compile: g++ -std=c++17 -O2 -Wall -Wextra -pedantic solution.cpp -o wizard\n";
    int T;
    cin >> T;
    Wizard Arawn;
    while(T--) {
        Spell *spell = Arawn.cast();
        counterspell(spell);
    }
    return 0;
}