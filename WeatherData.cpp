

#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <cmath>
using namespace std;

class WeatherData
{
protected:
    float temperature;
    float humidity;
    float windspeed;
    float pressure;
    string condition;
    string location;
    string timestamp;
    tm timeStruct;

public:
    WeatherData()
    {
        temperature = 0;
        humidity = 0;
        windspeed = 0;
        pressure = 0;
        location = "";
        timestamp = "";
        timeStruct = {}; // Initialize the time structure

        istringstream ss(timestamp);
        ss >> std::get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    }

    WeatherData(float temp, float hum, float wspeed, float pres, string loc, string wtime)
    {
        temperature = temp;
        humidity = hum;
        windspeed = wspeed;
        pressure = pres;
        location = loc;
        timestamp = wtime;
        timeStruct = {}; // Initialize the time structure

        istringstream ss(timestamp);
        ss >> std::get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    }
    void show()
    {
        cout << "\nTemperature : " << temperature 
             << "\tHumidity : " << humidity 
             << "\tWindspeed : " << windspeed << "\tPressure : " << pressure << "\tLocation : " << location
             << "\tTime : " << timestamp << endl;
    }

    void write(WeatherData &temp)
    {
        ofstream MyFile;
        MyFile.open("F:\\weather_management\\Data.txt", ios::app);
        if (MyFile.is_open())
        {
            cout << "File written successfully!\n";
            // Write data as text (human-readable)
            MyFile << temp.temperature << " "
                   << temp.humidity << " "
                   << temp.windspeed << " "
                   << temp.pressure << " "
                   << temp.location << " "
                   << temp.timestamp << "\n"; // Separate each record by a newline
        }
        else
        {
            cout << "Error writing file!\n";
        }

        MyFile.close();
    }

    void read(string place = "*", int month = 0)
    {
        WeatherData log;
        ifstream File;
        File.open("F:\\weather_management\\Data.txt", ios::in);

        if (File.is_open())
        {
            cout << "File opened successfully!\n";

            while (File >> log.temperature >> log.humidity >> log.windspeed >> log.pressure)
            {
                File.ignore(); // Ignore the space between pressure and location
                getline(File, log.location, ' ');
                getline(File, log.timestamp); // Read the rest of the line as the timestamp
                if (place == "*" && month == 0) 
            {
             log.show();  // Show all data
            }
            else if (place != "*" && month == 0 && log.location == place) 
            {
                log.show();  //  Filter by location
            }
            else if (place == "*" && month != 0 && log.timeStruct.tm_mon == month - 1) 
            {
                log.show();  //  Filter by month
            }
                 else if (log.location == place && log.timeStruct.tm_mon == month - 1) 
            {
                 log.show();  // Filter by location & month
            }

                
            }
        }
        else
        {
            cout << "Error opening file!\n";
        }

        File.close();
    }
};

class WeatherAnalyzer : public WeatherData
{
public:
    WeatherAnalyzer() // default constructor of weather analyzer
    {
    }

    WeatherAnalyzer(float temp, float hum, float wspeed, float pres, string loc, string wtime) : WeatherData(temp, hum, wspeed, pres, loc, wtime)
    {
    } // parameterised constructor
    WeatherAnalyzer readOne(ifstream &File)
    {
        WeatherAnalyzer log;

        if (File.is_open())
        {
            if (File >> log.temperature >> log.humidity >> log.windspeed >> log.pressure)
            {
                File.ignore(); // Ignore space
                getline(File, log.location, ' ');
                getline(File, log.timestamp);
            }
            else
            {
                cout << "End of File or Error in Reading!\n";
                File.close(); // File close kar do jab data khatam ho jaye
            }
        }
        else
        {
            cout << "Error opening file!\n";
        }

        return log;
    }

    void minmax()
    {
        float tempmax, tempmin, hummax, hummin, wsmax, wsmin, pmax, pmin;
        ifstream File;

        File.open("F:\\weather_management\\Data.txt", ios::in);

        if (!File.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }

        WeatherAnalyzer log = readOne(File);

        // Initialize min and max values with the first read data
        tempmax = tempmin = log.temperature;
        hummax = hummin = log.humidity;
        wsmax = wsmin = log.windspeed;
        pmax = pmin = log.pressure;

        while (File)
        {
            // Compare and update min/max for temperature
            if (log.temperature > tempmax)
                tempmax = log.temperature;
            if (log.temperature < tempmin)
                tempmin = log.temperature;

            // Compare and update min/max for humidity
            if (log.humidity > hummax)
                hummax = log.humidity;
            if (log.humidity < hummin)
                hummin = log.humidity;

            // Compare and update min/max for windspeed
            if (log.windspeed > wsmax)
                wsmax = log.windspeed;
            if (log.windspeed < wsmin)
                wsmin = log.windspeed;

            // Compare and update min/max for pressure
            if (log.pressure > pmax)
                pmax = log.pressure;
            if (log.pressure < pmin)
                pmin = log.pressure;

            // Read the next record
            log = readOne(File);
        }

        // Close the file after reading all records
        File.close();

        // Print the min/max results
        cout << "Temperature: Max = " << tempmax << ", Min = " << tempmin << endl;
        cout << "Humidity: Max = " << hummax << ", Min = " << hummin << endl;
        cout << "Windspeed: Max = " << wsmax << ", Min = " << wsmin << endl;
        cout << "Pressure: Max = " << pmax << ", Min = " << pmin << endl;
    }
    void averagedata(string place)
    {
        float tempavg = 0, humavg = 0, wpavg = 0, i = 0;
        ifstream File;
        File.open("F:\\weather_management\\Data.txt", ios::in);

        if (!File.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }
        while (File)
        {
            WeatherAnalyzer log = readOne(File);
            if (log.location == place)
            {
                tempavg += log.temperature;
                humavg += log.humidity;
                wpavg += log.windspeed;
                i++;
            }
        }
        tempavg = tempavg / i;
        humavg = humavg / i;
        wpavg = wpavg / i;
        cout << "\nAverage data for " << place << endl
             << "Temperature : " << tempavg << endl
             << "Humidity : " << humavg << endl
             << "WindSpeed : " << wpavg;
    }
    void standard_deaviation(string place)
    {
        float tempavg = 0, i = 0, variance = 0, t;
        ifstream file;
        vector<float> temparr;
        file.open("F:\\weather_management\\Data.txt", ios::in);
        while (file)
        {
            WeatherAnalyzer stemp = readOne(file);
            if (stemp.location == place)
            {
                t = stemp.temperature;
                i++;
                tempavg += t;
                temparr.push_back(t);
            }
        }
        file.close();
        tempavg = tempavg / i;
        for (float j : temparr)
        {
            variance += pow((j - tempavg), (float)2);
        }
        float k = float(temparr.size());
        variance = variance / k;
        float stdDev = sqrt(variance);

        cout << "\nTemperature Analysis:\n";
        cout << "Standard Deviation: " << stdDev << endl;
    }
    vector<float> calculaterateofchange(const vector<float> &data)
    {
        vector<float> rateofchange;
        for (size_t i = 1; i < data.size(); i++)
        {
            rateofchange.push_back((data[i]) - (data[i - 1]));
        }
        return rateofchange;
    }

    void detecttrend(const vector<float> rateofchange)
    {
        int risecount = 0, fallcount = 0;
        for (float i : rateofchange)
        {
            if (i > 0)
                risecount++;
            else
                fallcount++;
        }
        cout << "\ntreand analysis";
        if (risecount > fallcount)
        {
            cout << "\nrising trend";
        }
        if (risecount < fallcount)
        {
            cout << "\nfalling trend";
        }
        else
        {
            cout << "\nno clear trend";
        }
    }
    bool heatwave(const vector<float> &temperatures, float threshold, int days)
    {
        int consecutive_days = 0;
        for (float temp : temperatures)
        {
            if (temp > threshold)
            {
                consecutive_days++;
                if (consecutive_days >= days)
                {
                    return true;
                }
                else
                {
                    consecutive_days = 0;
                }
            }
        }
        return false;
    }

    void analyzeWeatherTrends(string place, int days, int month)
    {
        vector<float> tempdata, humiditydata, windspddata;
        ifstream File;
        File.open("F:\\weather_management\\Data.txt", ios::in);

        while (File)
        {
            WeatherAnalyzer log = readOne(File);
            istringstream ss(log.timestamp);
            ss >> std::get_time(&log.timeStruct, "%Y-%m-%d %H:%M:%S");
            if (log.location == place && (log.timeStruct.tm_mon == month - 1))
            {
                tempdata.push_back(log.temperature);
                humiditydata.push_back(log.humidity);
                windspddata.push_back(log.windspeed);
            }
        }
        File.close();
        if (static_cast<int>(tempdata.size()) < days)

        {
            cout << "Not enough data for " << days << " days of trend analysis!" << endl;
            return;
        }
        vector<float> temprate = calculaterateofchange(tempdata);
        detecttrend(temprate);
        if (heatwave(tempdata, 35, 3))
        {
            cout << "🔥 Heatwave Detected! 🔥\n";
        }
        if (heatwave(tempdata, 10, 3))
        {
            cout << "❄ Cold Spell Detected! ❄\n";
        }
        int rainy_days = 0;
        for (float hum : humiditydata)
        {
            if (hum > 80)
            {
                rainy_days++;
            }
        }
        if (rainy_days >= 3)
        {
            cout << "🌧 Rainy Period detected! 🌧\n";
        }
    }
};

int main()
{
    WeatherAnalyzer w;
    int ch, n;
    cout << "\n****WELCOME TO WEATHER LOGGER AND ANALYZER****\nA platform for ";
    while (1)
    {
        cout << "\nEnter your choice\t1->Enter data\t2->Display record\t3->Weather analysis\t4->Exit" << endl;
        cin >> ch;
        switch (ch)
        {
        case 1:
        {
            cout << "enter the no of data you want to enter : ";
            cin >> n;

            float t, h, ws, p;
            string loc, wtime;

            for (int i = 0; i < n; i++)
            {
                cout << "enter the temperature : ";
                cin >> t;
                cout << "enter the humidity : ";
                cin >> h;
                cout << "enter the windspeed : ";
                cin >> ws;
                cout << "enter the pressure : ";
                cin >> p;
                cout << "enter the location : ";
                cin >> loc;
                fflush(stdin);

                cout << "enter the timestamp : ";

                getline(cin, wtime);
                w = WeatherAnalyzer(t, h, ws, p, loc, wtime);
                w.write(w);
            }
            break;
        }
        case 2:
        {
            int c, mon;
            string place;
            cout << endl
                 << "Enter\n 1->Display Full record \t2->Display by applying filter :\n";
            cin >> c;
            if (c == 1)
                w.read();
            else
            {
                cout << "Enter month to be searched (0 if not applied ) : ";
                cin >> mon;
                cout << "Enter place to be searched (* if not applied ) : ";
                cin >> place;
                w.read(place, mon);
            }
            break;
        }
        case 3:
        {
            string place;
            int month;
            cout << " \nEnter Place : ";
            cin >> place;
            cout << "\nenter month";
            cin >> month;
            w.minmax();
            w.averagedata(place);
            w.analyzeWeatherTrends(place, 3, month);
            return 0;
            break;
        }
        case 4:
        {
            exit(0);
        }
        }
    }
}