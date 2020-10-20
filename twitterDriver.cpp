#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include "Twitter.h"
#include "User.h"
#include "Tweet.h"
#include <unistd.h>

std::string getline_user_response();
std::string cin_user_response();
bool compose_tweet(Twitter &t);
bool get_user_choice();
std::string get_date();
std::string str_to_lower(std::string str);
void twitter_ui(Twitter &t);
void clear_in();

int main()
{
    //response dictates which constructor to use
    std::cout << "Do you have a Twitter file to load (y/n)? ";
    bool check = get_user_choice(); //prompts user for y/n returns 1/0
    clear_in();
    if (check)
    {
        std::cout << "Enter the filename: ";
        std::string filename = getline_user_response();
        std::ifstream fin;
        fin.open(filename);

        if (fin.fail())
        {
            std::cout << "File not found. Let's start from scratch." << std::endl;
            std::cout << "Enter your username here: ";
            std::string temp_id;
            std::cin >> temp_id;
            Twitter t(temp_id);
            clear_in();
            twitter_ui(t);
            bool for_human = false;
            t.xport("twitter_profile", for_human);
            return 0;
        }
        std::cout << "Profile loaded." << std::endl;
        Twitter t(fin);
        std::cout << "Hello, and welcome to the Twitter UI!" << std::endl;
        std::cout << "Enter the name of the user you would like to log in as: ";
        t.change_user(getline_user_response());
        twitter_ui(t);
        std::cout << "Would you like to print your tweets to a file (y/n)? ";
        bool want_to_print = get_user_choice();
        if (want_to_print)
        {
            bool for_human = true;
            std::cout << "Now exporting your tweets to a txt file called \"your_tweets\"." << std::endl;
            t.xport("your_tweets", for_human);
        }
        bool for_human = false;
        t.xport("twitter_profile", for_human);
        return 0;
    }
    else
    {
        //make a new twitter object with user input ID
        std::cout << "OK. Lets make you a new profile. Enter your username here: ";
        std::string temp_id;
        std::cin >> temp_id;
        Twitter t(temp_id);

        //To the UI!
        std::cout << std::endl;
        std::cout << "Lets try the UI: " << std::endl;
        clear_in();
        twitter_ui(t);
        std::cout << std::endl;

        //Print_all() call
        bool for_human = false;
        t.xport("twitter_profile", for_human);
    }
}

//compose tweet function
bool compose_tweet(Twitter &t)
{
    //accept the tweet
    std::cout << "Enter your Tweet (terminate tweet with '*'):";
    std::string tweet;
    std::getline(std::cin, tweet, '*');
    bool check;

    //see if the tweet is too long
    while (tweet.length() > 280)
    {
        std::cout << "That tweet is too long. Tweets must be under 280 characters in length." << std::endl;
        std::cout << "Would you like to enter a shorter tweet (y/n)? ";
        check = get_user_choice();
        if (check)
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Ok. Enter your new Tweet (Terminate tweet with '*'): " << std::endl;
            std::getline(std::cin, tweet);
        }
        else
        {
            std::cout << "Tweet canceled." << std::endl;
            return 0;
        }
    }

    //build a temp tweet
    std::cout << std::endl;
    std::string date = get_date();
    Tweet temp(t.get_current_userID(), tweet, date);

    //show it to them
    std::cout << "Great! Your Tweet looks like this:" << std::endl;
    std::cout << temp.to_tweet();
    std::cout << std::endl;

    //confirm to publish
    std::cout << "Would you like to publish your Tweet (y/n)? ";
    check = get_user_choice();
    if (check)
    {
        t.write_tweet(t.get_current_userID(), tweet, date);
        std::cout << "Your Tweet has been published." << std::endl;
        std::cout << std::endl;
        clear_in();
        return 1;
    }
    else
    {
        std::cout << "Tweet canceled." << std::endl;
        std::cout << std::endl;
        return 0;
    }
}

std::string get_date()
{
    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    std::string dt = ctime(&now);

    //truncate to remove timestamp
    std::string date = dt.substr(0, 10);

    return date;
}

//keeps running and accepting input
////TODO:
//input stream gets derailed due to the ignore statements
//maybe put them in a seperate function?

void twitter_ui(Twitter &t)
{
    std::cout << std::endl;
    std::cout << std::endl;
    usleep(1500000);
    std::cout << "Great - here's a rundown of the available commands: " << std::endl;
    std::cout << std::endl;
    usleep(2000000);
    std::cout << "You can type\n\"new user\" to create a new user,";
    usleep(1000000);
    std::cout << "\n\"tweet\" to write a tweet,";
    usleep(1000000);
    std::cout << "\n\"change user\" to change the current user,";
    usleep(1000000);
    std::cout << "\n\"follow\" to follow another user,";
    usleep(1000000);
    std::cout << "\n\"unfollow\" to follow another user, or";
    usleep(1000000);
    std::cout << "\n\"timeline\" to see the tweets of all of the users you follow. ";
    usleep(1000000);
    std::cout << "\nyou can also type \"load profile\" to load a Twitter profile from a Twitter generated txt document. ";
    usleep(1000000);
    std::cout << "\nWhen you're finished, type \"end\" to generate a Twitter text file and quit the progam.";
    usleep(1000000);
    std::cout << std::endl;
    std::string user_command;
    bool message = false; //suppresses secondary loop message when neccesary
    while (true)
    {
        if (message)
        {
            usleep(1500000);
            std::cout << std::endl;
            std::cout << "Type\n'new user' to create a new user,"
                         "\n'tweet' to write a tweet,\n'change user' to change the current user,"
                         "\n'follow' to follow another user,\n'timeline' to see the tweets of"
                         " all of the users you follow,"
                         " or\n'end' to generate a Twitter text file and quit the progam."
                      << std::endl;
        }
        std::cout << std::endl;
        std::cout << "What would you like to do?" << std::endl;
        user_command = getline_user_response();
        user_command = str_to_lower(user_command);

        //call the appropriate function
        if (user_command == "tweet")
        {
            compose_tweet(t);
            message = true;
        }
        else if (user_command == "new user")
        {
            std::string new_uID;
            std::cout << "Enter the username for the profile you would like to create: ";
            new_uID = cin_user_response();
            clear_in();
            t.new_user(new_uID);
            message = true;
        }
        else if (user_command == "change user")
        {
            std::string change_uID;
            std::cout << "Enter the username of the profile you would like to login to: ";
            change_uID = cin_user_response();
            clear_in();
            t.change_user(change_uID);
            message = true;
        }
        else if (user_command == "follow")
        {
            std::string follow_uID;
            std::cout << "Enter the user ID of the user you would like to follow: ";
            follow_uID = cin_user_response();
            clear_in();
            t.follow(follow_uID);
            message = true;
        }
        else if (user_command == "unfollow")
        {
            std::string unfollow_uID;
            std::cout << "Enter the ID of the user you would like to unfollow: ";
            unfollow_uID = cin_user_response();
            clear_in();
            t.unfollow(unfollow_uID);
            message = true;
        }
        else if (user_command == "timeline")
        {
            std::cout << "Awesome - Here's your timeline: " << std::endl;
            std::cout << std::endl;
            t.timeline();
            message = true;
        }
        else if (user_command == "load profile")
        {
            std::cout << "Enter the filename: ";
            std::string filename = getline_user_response();
            std::ifstream fin;
            fin.open(filename);
            if (fin.fail())
            {
                std::cout << "Could not find that Twitter txt file." << std::endl;
                continue;
            }
            std::cout << "Profile updated!" << std::endl;
            t.from_file(fin);
            message = true;
        }
        else if (user_command == "end")
        {
            break;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "Invalid response. Accepted responses are \n'new user' to create a new user,"
                         "\n'tweet' to write a tweet,\n'change user' to change the current user,"
                         "\n'follow' to follow another user,\n'timeline' to see the tweets of"
                         " all of the users you follow,"
                         " or\n'end' to generate a Twitter text file and quit the progam."
                      << std::endl;
            std::cout << "Please enter a valid response. " << std::endl;
            message = false;
        }
    }
    std::cout << "Thanks for using twitter! Now generating twitter_profile.txt."
                 " Load this profile into the program next time it runs to pick up where you left off."
              << std::endl;
    return;
}

std::string getline_user_response()
{
    std::string str;
    std::getline(std::cin, str);
    return str;
}

std::string cin_user_response()
{
    std::string str;
    std::cin >> str;
    return str;
}
