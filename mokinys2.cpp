#include "mokinys2.h"
#include "funkcijos2.h"

bool Patikrinimas(string kint)
{
    const int ilgis = kint.length();
    char *temp_array = new char[ilgis + 1];
    strcpy(temp_array, kint.c_str());
    for (int i = 0; i < ilgis; i++)
    {
        if (!isalpha(temp_array[i]))
            return false;
    }
    delete[] temp_array;
    return true;
}

void Mokinys :: Vidurkis(deque<Mokinys> &A)
{
    for (int i = 0; i < A.size(); i++)
    {
        Mokinys &mok = A[i];
        double suma = 0;
        for (int pazymys : mok.ND)
        {
            suma += pazymys;
        }
        suma += mok.egzaminas;
        double vidurkis = suma / (mok.ND.size() + 1);
        mok.VID = vidurkis;
    }
}

bool Mokinys ::  PagalVidurki(const Mokinys &a, const Mokinys &b)
{
    return a.VID < b.VID;
}

bool  Mokinys ::  PagalMediana(const Mokinys &a, const Mokinys &b)
{
    return a.MED < b.MED;
}

bool Mokinys :: PagalVarda(const Mokinys &a, const Mokinys &b)
{
    return a.vardas < b.vardas;
}

bool Mokinys ::  PagalPavarde(const Mokinys &a, const Mokinys &b)
{
    return a.pavarde < b.pavarde;
}

double visasLaikas;

void GeneruotiFailus(deque<Mokinys> &Nuskriaustieji, deque<Mokinys> &Mokslinciai, deque<int> &IrasuSk, deque<Mokinys> &A)
{
    srand(time(NULL));

    for (int i = 0; i < IrasuSk.size(); i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto st = start;
        cout << "~FAILAS " + to_string(i) + "~" << endl;
        cout << "Genruojama..." << endl;
        string filename = "new_file" + to_string(i) + ".txt";
        ofstream fr(filename);

        if (!fr)
            throw runtime_error("Failas nurodytoje vietoje neegzistuoja!");

        int temp = rand() % 10 + 1;

        fr << setw(20) << left << "Vardas" << setw(20) << left << "Pavarde";
        for (int i = 0; i < temp; i++)
            fr << setw(10) << left << "ND";
        fr << setw(10) << left << "Egz." << endl;

        for (int j = 0; j < IrasuSk[i]; j++)
        {
            fr << setw(20) << left << ("Vardas" + to_string(j)) << setw(20) << left << ("Pavarde" + to_string(j));
            for (int k = 0; k < temp; k++)
                fr << setw(10) << left << rand() % 10 + 1;
            fr << setw(10) << left << rand() % 10 + 1 << endl;
        }

        fr.close();

        //Skaitymas(Nuskriaustieji, Mokslinciai, IrasuSk, filename, A, i);
    }
}

void Mokinys :: Skaitymas(deque<Mokinys> &Nuskriaustieji, deque<Mokinys> &Mokslinciai, deque<int> &IrasuSk, string failas, deque<Mokinys> &A, int &temp, char strategija)
{
    visasLaikas = 0.0;

    string eil;
    ifstream fd(failas);
    if (!fd)
        throw runtime_error("Failas nurodytoje vietoje neegzistuoja!");
    getline(fd, eil);
    auto start = std::chrono::high_resolution_clock::now();
    auto st = start;
    while (getline(fd, eil))
    {
        istringstream iss(eil);
        Mokinys temp;
        iss >> temp.vardas >> temp.pavarde;
        int pazymiai;
        while (iss >> pazymiai)
            temp.ND.push_back(pazymiai);

        if (!temp.ND.empty())
        {
            temp.egzaminas = temp.ND.back();
            temp.ND.pop_back();
        }

        sort(temp.ND.begin(), temp.ND.end());
        double mediana;

        if (!temp.ND.empty())
        {
            mediana = temp.ND[temp.ND.size() / 2];
            if (temp.ND.size() % 2 == 0)
            {
                mediana = (temp.ND[temp.ND.size() / 2 - 1] + temp.ND[temp.ND.size() / 2]) / 2.0;
            }
        }
        temp.MED = mediana;
        A.push_back(temp);
    }
    fd.close();
    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Nuskaitymo laikas: " << diff.count() << "s\n";
    visasLaikas += diff.count();

    Vidurkis(A);
    if (strategija == '1')
        StudentuRusiavimas(Nuskriaustieji, Mokslinciai, A, IrasuSk, failas, temp);
    if (strategija == '2')
        StudentuRusiavimas2(Nuskriaustieji, Mokslinciai, A, IrasuSk, failas, temp);
    if (strategija == '3')
        StudentuRusiavimas3(Nuskriaustieji, Mokslinciai, A, IrasuSk, failas, temp);
    cout << endl;
}

void Mokinys :: StudentuRusiavimas(deque<Mokinys> &Nuskriaustieji, deque<Mokinys> &Mokslinciai, deque<Mokinys> &A, deque<int> &IrasuSk, string failas, int &temp)
{
    string filename = "nuskriaustieji" + to_string(temp) + ".txt";
    string filename1 = "mokslinciai." + to_string(temp) + ".txt";

    int kint;
    auto start = std::chrono::high_resolution_clock::now();
    auto st = start;

    sort(A.begin(), A.end(), PagalVidurki);

    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Studentu rusiavimas didejimo tvarka uztruko: " << diff.count() << "s\n";
    visasLaikas +=diff.count();

    auto start1 = std::chrono::high_resolution_clock::now();
    auto st1 = start1;

    for (auto &mok : A)
    {
        if (mok.VID > 5.0)
            Nuskriaustieji.push_back(mok);
        else
            Mokslinciai.push_back(mok);
    }

    std::chrono::duration<double> diff1 = std::chrono::high_resolution_clock::now() - start1;
    cout << "Studentu rusiavimas uztruko: " << diff1.count() << "s\n";
    visasLaikas += diff1.count();

    cout << "Visa programa " + to_string(temp) + " uztruko: " << visasLaikas << "s\n";

    Isvedimas2(Mokslinciai, Mokslinciai.size(), filename);
    Isvedimas2(Nuskriaustieji, Nuskriaustieji.size(), filename1);
}
void Mokinys :: StudentuRusiavimas2(deque<Mokinys> &Nuskriaustieji, deque<Mokinys> &Mokslinciai, deque<Mokinys> &A, deque<int> &IrasuSk, string failas, int &temp)
{
    string filename = "nuskriaustieji" + to_string(temp) + ".txt";
    string filename1 = "mokslinciai." + to_string(temp) + ".txt";

    int kint;
    auto start = std::chrono::high_resolution_clock::now();
    auto st = start;

    sort(A.begin(), A.end(), PagalVidurki);

    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Studentu rusiavimas didejimo tvarka uztruko: " << diff.count() << "s\n";
    visasLaikas +=diff.count();

    auto start1 = std::chrono::high_resolution_clock::now();
    auto st1 = start1;

    for (int i=A.size() - 1; i >= 0; i--)
    {
        if (A[i].VID > 5.0){
            Nuskriaustieji.push_back(A[i]);
            A.pop_back();
        }
    }

    std::chrono::duration<double> diff1 = std::chrono::high_resolution_clock::now() - start1;
    cout << "Studentu rusiavimas uztruko: " << diff1.count() << "s\n";
    visasLaikas += diff1.count();

    cout << "Visa programa " + to_string(temp) + " uztruko: " << visasLaikas << "s\n";

    Isvedimas2(A, A.size(), filename);
    Isvedimas2(Nuskriaustieji, Nuskriaustieji.size(), filename1);
}
void Mokinys :: StudentuRusiavimas3(deque<Mokinys> &Nuskriaustieji, deque<Mokinys> &Mokslinciai, deque<Mokinys> &A, deque<int> &IrasuSk, string failas, int &temp) {
    string filename = "nuskriaustieji" + to_string(temp) + ".txt";
    string filename1 = "mokslinciai." + to_string(temp) + ".txt";

    auto start = chrono::high_resolution_clock::now();

    sort(A.begin(), A.end(), PagalVidurki);

    chrono::duration<double> diff = chrono::high_resolution_clock::now() - start;
    cout << "Studentu rusiavimas didejimo tvarka uztruko: " << diff.count() << "s\n";

    auto start1 = chrono::high_resolution_clock::now();

    auto nuskriaustieji_begin = stable_partition(A.begin(), A.end(), [](const Mokinys &mok) {
        return mok.VID <= 5.0;
    });

    deque<Mokinys> tempDeque(make_move_iterator(nuskriaustieji_begin), make_move_iterator(A.end()));
    A.erase(nuskriaustieji_begin, A.end());
    Nuskriaustieji.insert(Nuskriaustieji.end(), make_move_iterator(tempDeque.begin()), make_move_iterator(tempDeque.end()));

    chrono::duration<double> diff1 = chrono::high_resolution_clock::now() - start1;
    cout << "Studentu rusiavimas uztruko: " << diff1.count() << "s\n";

    cout << "Visa programa " + to_string(temp) + " uztruko: " << diff.count() + diff1.count() << "s\n";

    Isvedimas2(Mokslinciai, Mokslinciai.size(), filename);
    Isvedimas2(Nuskriaustieji, Nuskriaustieji.size(), filename1);
}
void Mokinys :: Isvedimas(const deque<Mokinys> &A, int MOK_kiekis, string isvedimas)
{
    char kint;
    cout << "Rezultatus matyti norite ekrane ar faile?(e/f): ";
    cin >> kint;
    if (kint == 'e')
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto st = start;
        cout << setw(20) << left << "Vardas" << setw(20) << left << "Pavarde" << setw(20) << right << "Galutinis (Vid.) / Galutinis(Med.)" << endl;
        cout << setfill('-') << setw(80) << " " << endl;
        cout << setfill(' ');

        for (int i = 0; i < MOK_kiekis; i++)
        {
            cout << setw(20) << left << A[i].vardas << setw(20) << left << A[i].pavarde << right << fixed << setprecision(2) << A[i].VID << setw(18) << right << A[i].MED;
            cout << endl;
        }
        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
        cout << "Studentu isvedimas ekrane uztruko: " << diff.count() << "s\n";
    }
    else if (kint == 'f')
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto st = start;
        ofstream fr(isvedimas);
        fr << setw(20) << left << "Vardas" << setw(20) << left << "Pavarde" << setw(20) << right << "Galutinis (Vid.) / Galutinis(Med.)" << endl;
        fr << setfill('-') << setw(80) << " " << endl;
        fr << setfill(' ');

        for (int i = 0; i < MOK_kiekis; i++)
        {
            fr << setw(20) << left << A[i].vardas << setw(20) << left << A[i].pavarde << right << fixed << setprecision(2) << A[i].VID << setw(18) << right << A[i].MED;
            fr << endl;
        }
        fr.close();
        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
        cout << "Studentu isvedimas i failus uztruko: " << diff.count() << "s\n";
    }
    else
        throw runtime_error("Netinkama ivestis!");
}

void Mokinys :: Isvedimas2(const deque<Mokinys> &A, int MOK_kiekis, string isvedimas)
{
  /*   auto start = std::chrono::high_resolution_clock::now();
    auto st = start; */
    ofstream fr(isvedimas);
    fr << setw(20) << left << "Vardas" << setw(20) << left << "Pavarde" << setw(20) << right << "Galutinis (Vid.) / Galutinis(Med.)" << endl;
    fr << setfill('-') << setw(80) << " " << endl;
    fr << setfill(' ');

    for (int i = 0; i < MOK_kiekis; i++)
    {
        fr << setw(20) << left << A[i].vardas << setw(20) << left << A[i].pavarde << right << fixed << setprecision(2) << A[i].VID << setw(18) << right << A[i].MED;
        fr << endl;
    }
    fr.close();
    /* std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Studentu isvedimas i failus uztruko: " << diff.count() << "s\n"; */
}

void Mokinys :: Rikiavimas(deque<Mokinys> &Mokslinciai, deque<Mokinys> &Nuskriaustieji, deque<int> &IrasuSk)
{
    char kint;
    cout << "Pagal ka rikiuoti: varda, pavarde, vidurki, mediana?(v, p, V, m)" << endl;
    cin >> kint;
    if (kint == 'V')
    {
        sort(Mokslinciai.begin(), Mokslinciai.end(), PagalVidurki);
        sort(Nuskriaustieji.begin(), Nuskriaustieji.end(), PagalVidurki);
    }
    else if (kint == 'm')
    {
        sort(Mokslinciai.begin(), Mokslinciai.end(), PagalMediana);
        sort(Nuskriaustieji.begin(), Nuskriaustieji.end(), PagalMediana);
    }
    else if (kint == 'v')
    {
        sort(Mokslinciai.begin(), Mokslinciai.end(), PagalVarda);
        sort(Nuskriaustieji.begin(), Nuskriaustieji.end(), PagalVarda);
    }
    else if (kint == 'p')
    {
        sort(Mokslinciai.begin(), Mokslinciai.end(), PagalPavarde);
        sort(Nuskriaustieji.begin(), Nuskriaustieji.end(), PagalPavarde);
    }
    else
        throw runtime_error("Netinkama ivestis!");
}