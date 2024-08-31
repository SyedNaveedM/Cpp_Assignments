#include<bits/stdc++.h>
#include "class.h"
using namespace std;

// Function to extract individual strings from the quotes in the input string
string extract(const string& input, size_t& startPos) 
{
    size_t firstQuote = input.find('"', startPos) + 1; // Position after the first quote
    size_t secondQuote = input.find('"', firstQuote);  // Position of the closing quote
    startPos = secondQuote + 1;  // Update startPos for the next search
    return input.substr(firstQuote, secondQuote - firstQuote);
}

bool isValidDate(string date)
{
    // Check if the date has the correct format
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
    {
        return false;
    }

    // Extract year, month, and day
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // Check if year, month, and day are valid
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        return false;
    }

    // Check if the day is valid for the month
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return false;
    }
    else if (month == 2)
    {
        // Check for leap year
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            if (day > 29)
            {
                return false;
            }
        }
        else if (day > 28)
        {
            return false;
        }
    }

    return true;
}

// Function for splitting the location for extracting address, country etc
vector<string> split(const string& input, char delimiter) 
{
    vector<string> tokens;
    stringstream ss(input);
    string token;

    while (getline(ss, token, delimiter)) 
    {
        tokens.push_back(token);
    }

    return tokens;
}

int main()
{
    // For input checking
    string addc="addCongregation";
    string delc="deleteCongregation";
    string showc="showCongregations";
    string addv="addVenue";
    string delv="deleteVenue";
    string showv="showVenues";
    string resv="reserveVenue";
    string freev="freeVenue";
    string showr="showReserved";
    string adde="addEvent";
    string dele="deleteEvent";
    string showe="showEvents";
    string showcal="showCalendar";
    string end="End";

    // Vector storing the congregations
    vector<cong> Cong;
    vector<venue> Ven;
    vector<event> Eve;

    // Run an infinite loop till we encounter End
    while(1)
    {
        string input,x;
        getline(cin,input);
        // This will extract the first word of the sentence
        string command;
        istringstream stream(input);
        stream>>command;

        // End the loop if the input entered is End
        if(command==end)
        {
            break;
        }

        // Now start checking what is the command entered
        if(command==addc)
        {
            // Call addCongregation
            string name;
            string type;
            string start_date;
            string end_date;

            size_t start=input.find(' ');

            // Extract the quoted strings
            name = extract(input,start);
            type = extract(input,start);
            start_date = extract(input,start);
            end_date = extract(input,start);

            if(isValidDate(start_date) and isValidDate(end_date))
            {
                // Make a object of congregation class

            }
            else 
            {
                cout<<"-1\nError\n";
            }
        }
        else if(command==delc)
        {
            // Call deleteCongregation


            string name;

            size_t start=input.find(' ');

            name=extract(input,start);


            // cout<<command<<" "<<name<<"\n";
        }
        else if(command==showc)
        {
            // Call showcongregation
            // cout<<command<<"\n";
        }
        else if(command==addv)
        {
            // Call addvenue
            string name;
            string Location;
            string capacity;

            size_t start=input.find(' ');
            name=extract(input,start);
            Location=extract(input,start);
            capacity=input.substr(start);

            char delimit=':';
            vector<string> location=split(Location,delimit);

            // cout<<command<<" "<<name<<" "<<capacity<<"\n";
            // for(auto i:location)
            // {
            //     cout<<i<<" ";
            // }
            // cout<<"\n";
        }
        else if(command==delv)
        {
            // Call deletevenue
            string name;
            string country;
            size_t start=input.find(' ');
            name=extract(input,start);
            country=extract(input,start);

            // cout<<command<<" "<<name<<" "<<country<<"\n";
        }
        else if(command==showv)
        {
            // Call showvenues
            size_t start=input.find(' ');

            string Location;
            Location=extract(input,start);

            // Split the string
            char delimit=':';
            vector<string> location=split(Location,delimit);
            // cout<<command<<" ";
            // for(auto i:location)
            // {
            //     cout<<i<<" ";
            // }
            // cout<<"\n";
        }
        else if(command==resv)
        {
            // Call reservevenue
            size_t start=input.find(' ');

            string vname;
            string vcountry;
            string cname;

            vname=extract(input,start);
            vcountry=extract(input,start);
            cname=extract(input,start);

            // cout<<command<<" "<<vname<<" "<<vcountry<<" "<<cname<<"\n";
        }
        else if(command==freev)
        {
            // Call freevenue
            size_t start=input.find(' ');

            string vname;
            string vcountry;
            string cname;

            vname=extract(input,start);
            vcountry=extract(input,start);
            cname=extract(input,start);

            // cout<<command<<" "<<vname<<" "<<vcountry<<" "<<cname<<"\n";
        }
        else if(command==showr)
        {
            // Call showreserved
            size_t start=input.find(' ');

            string cname;

            cname=extract(input,start);

            // cout<<command<<" "<<cname<<"\n";
        }
        else if(command==adde)
        {
            // Call addevent
            size_t start=input.find(' ');

            string cname;
            string vname;
            string vcountry;
            string date;
            string fromTime;
            string toTime;
            string ename;

            cname=extract(input,start);
            vname=extract(input,start);
            vcountry=extract(input,start);
            date=extract(input,start);
            fromTime=extract(input,start);
            toTime=extract(input,start);
            ename=extract(input,start);

            // cout<<command<<" "<<" "<<vname<<" "<<vcountry<<" "<<date<<" "<<fromTime<<" "<<toTime<<" "<<ename<<"\n";
        }
        else if(command==dele)
        {
            // Call deleteevent
            size_t start=input.find(' ');

            string cname;
            string vname;
            string vcountry;
            string date;
            string fromTime;
            string ename;

            cname=extract(input,start);
            vname=extract(input,start);
            vcountry=extract(input,start);
            date=extract(input,start);
            fromTime=extract(input,start);
            ename=extract(input,start);

            if(isValidDate(date))
            {
                // Make object
            }
            else
            {
                cout<<"-1\nError\n";
            }
            // cout<<command<<" "<<" "<<vname<<" "<<vcountry<<" "<<date<<" "<<fromTime<<" "<<ename<<"\n";
        }
        else if(command==showe)
        {
            // Call showevents
            size_t start=input.find(' ');

            string vname;
            string vcountry;
            string date;

            vname=extract(input,start);
            vcountry=extract(input,start);
            date=extract(input,start);

            if(isValidDate(date))
            {
                // Make object
            }
            else
            {
                cout<<"-1\nError\n";
            }

            // cout<<command<<" "<<vname<<" "<<vcountry<<" "<<date<<"\n";
        }
        else if(command==showcal)
        {
            // Call showcalendar
            size_t start=input.find(' ');

            string cname;
            string vname;
            string vcountry;

            cname=extract(input,start);
            vname=extract(input,start);
            vcountry=extract(input,start);

        }
        else 
        {
            cout<<"-1\nError\n";
        }
    }
}
