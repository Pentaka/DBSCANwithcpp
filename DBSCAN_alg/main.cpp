#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// Veri yapisini olustur
struct DataPoint {
    vector<double> veri_ozellikleri; // veri özellikleri
    int kume; // hangi kümeye ait oldugunu tutan etiket
};

// Veri okuma islemleri için fonksiyon
vector<DataPoint> readData(string filename) {
    vector<DataPoint> data;
    ifstream file(filename);
    string satir;
    while (getline(file, satir)) {
        DataPoint dp;
        stringstream ss(satir);
        string ozellik;
        while (getline(ss, ozellik, ',')) {
            dp.veri_ozellikleri.push_back(stod(ozellik));//veri okuyup double'a dönüstürme
        }
        dp.kume = -1; // baslangicta her veri noktasi bir kümede olmadigi için -1
        data.push_back(dp);//dp veri noktasini dataya ekleme
    }

    return data;
}

// Veri noktalari arasindaki mesafeyi hesaplayan fonksiyon
double uzaklik(DataPoint p1, DataPoint p2) {//öklid ile uzaklik
    double uzak = 0.0;
    for (int i = 0; i < p1.veri_ozellikleri.size(); i++) {
        uzak += pow(p1.veri_ozellikleri[i] - p2.veri_ozellikleri[i], 2);
    }
    return sqrt(uzak);
}

// Eps parametresine göre veri noktalarinin komsularini bulan fonksiyon
vector<int> getkomsular(vector<DataPoint> data, int idx, double eps) {
vector<int> komsular;
    for (int i = 0; i < data.size(); i++) {
        if (i != idx && uzaklik(data[idx], data[i]) <= eps) {
            komsular.push_back(i);
        }
    }
    return komsular;
}

void dbscan(vector<DataPoint>& data, double eps, int minPts) {
    int kume = 0;
    vector<vector<int>> kumeler;
    vector<int> gurultu;
    vector<bool> isaretlenen(data.size(), false);

    for (int i = 0; i < data.size(); i++) {
        if (data[i].kume == -1) {
            vector<int> komsular = getkomsular(data, i, eps);
            vector<int> kumeAdayi = {i};
            bool isaretlendi = false;

            for (int j = 0; j < komsular.size(); j++) {
                int komsuIdx = komsular[j];

                if (!isaretlenen[komsuIdx]) {
                    isaretlenen[komsuIdx] = true;
                    vector<int> komsuKomsular = getkomsular(data, komsuIdx, eps);

                    if (komsuKomsular.size() >= minPts) {
                        kumeAdayi.push_back(komsuIdx);
                    }

                    if (data[komsuIdx].kume != -1) {
                        isaretlendi = true;
                    }
                }
            }

            if (!isaretlendi) {
                if (kumeAdayi.size() < minPts) {
                    gurultu.push_back(i);
                } else {
                    kume++;
                    data[i].kume = kume;
                    kumeler.push_back(kumeAdayi);

                    for (int j = 0; j < kumeAdayi.size(); j++) {
                        int kumeAdayiIdx = kumeAdayi[j];
                        data[kumeAdayiIdx].kume = kume;
                    }
                }
            }
        }
    }


    // gurultu kümesi icin cikti
if (gurultu.size() > 0) {
    cout << "Gurultu Kumesi (" << gurultu.size() << " eleman) ";
    cout << endl;
}

// Diger kumeler icin cikti
for (int i = 0; i < kumeler.size(); i++) {
    if (kumeler[i].size() < minPts) {
        gurultu.insert(gurultu.end(), kumeler[i].begin(), kumeler[i].end());
        continue;
    }
    cout << "Kume " << i+1 << " (" << kumeler[i].size() << " eleman) " << endl;
}


}
int main() {
string filename = "veri.csv";
double eps;
int minPts;
cout <<"eps: ";
cin >> eps;
cout<<"minPts:";
cin>>minPts;

vector<DataPoint> data = readData(filename);
dbscan(data, eps, minPts);

return 0;
}

