#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "User.h"
#include "Tweet.h"
#include "Twitter.h"
#include <ctype.h>

bool get_user_choice();
std::string str_to_lower(std::string str);
void clear_in();

Twitter::Twitter()
{
    User temp;
    current_userID = temp.get_userID();
}

Twitter::Twitter(std::string uID)
{
    new_user(uID);
    user_index = 0;
    current_userID = user_vec[user_index].get_userID();
}

//creates new twitter object from passed file
Twitter::Twitter(std::ifstream &fin)
{
    from_file(fin);
}

void Twitter::from_file(std::ifstream &fin)
{
    /* grabs the block info (username, tweets) for one user
    at a time, sends that to user_reader */
    std::string user_info;
    while (std::getline(fin, user_info, '#'))
    {
        std::stringstream ss;
        ss << user_info;
        user_reader(ss);
    }
}

/* grabs the username of each user (first line)
and sends the rest (the tweets) to tweet_reader */
void Twitter::user_reader(std::stringstream &ss)
{
    std::string str;
    std::string uID;
    std::getline(ss, uID); //grab the userID
    bool check_tweets = false;
    check_tweets = new_user_fin(uID);

    while (std::getline(ss, str, '$'))
    {
        /* apparently this filters out newlines
        don't remember if this is actually a problem */
        if (str == "\n")
        {
            continue;
        }
        //export tweet as stringstream to tweet_reader
        std::stringstream tweet;
        tweet << str;
        tweet_reader(tweet, check_tweets);
    }
}

/* reads individual tweets and separates them into
username, date, and tweet */
void Twitter::tweet_reader(std::stringstream &tweet, bool check_tweets)
{
    //vector of strings
    std::vector<std::string> tweet_member;
    std::string temp;
    int i = 0;
    while (std::getline(tweet, temp, '*'))
    {
        //skip over the newlines and empty strings
        if (temp == "\n" || temp == "")
        {
            continue;
        }
        tweet_member.push_back(temp);
        i++;
    }
    //finally we can create the tweet
    std::string date = tweet_member[0];
    std::string uID = tweet_member[1];
    std::string twt = tweet_member[2];
    if (check_tweets)
    {
        write_tweet(uID, twt, date, check_tweets);
        return;
    }
    write_tweet(uID, twt, date);
    return;
}

//new user function for file input
//creates user and switches to that user's account
bool Twitter::new_user_fin(std::string uID)
{
    for (int i = 0; i < user_vec.size(); i++)
    {
        if (uID == user_vec[i].get_userID())
        {
            user_index = i;
            return true;
        }
    }
    User temp(uID);
    user_vec.push_back(temp);
    user_index = user_vec.size() - 1;
    return false;
}

//calls new_tweet with passed uID, tweet body, and date
void Twitter::write_tweet(std::string uID, std::string twt, std::string dte)
{
    user_vec[user_index].new_tweet(uID, twt, dte);
}

//creates a twitter object and a user object with passed id
void Twitter::new_user(std::string uID)
{
    //make sure
    int i = find_index(uID);
    if (i == -1)
    {
        User temp(uID);
        user_vec.push_back(temp);
        int temp_index = user_vec.size() - 1;
        std::cout << "User " << user_vec[temp_index].get_userID() << " created." << std::endl;
        return;
    }

    std::cout << "That user already exists. Would you like to switch to " << user_vec[i].get_userID() << "'s account?" << std::endl;
    bool check = get_user_choice();
    if (check)
    {
        change_user(user_vec[i].get_userID());
        std::cout << "Now in " << user_vec[user_index].get_userID() << "'s account." << std::endl;
        return;
    }
    std::cout << "Still in " << user_vec[user_index].get_userID() << "'s account." << std::endl;
}

/* changes user to passed ID
if no matching user exists, prompts to create user with passed ID 
returns 0 if no matching user exists and user declines to create new account */
bool Twitter::change_user(std::string uID)
{
    //try to find a user with matching user ID
    for (int i = 0; i < user_vec.size(); i++)
    {
        if (uID == user_vec[i].get_userID())
        {
            if (i == user_index)
            {
                std::cout << "You are already in " << user_vec[user_index].get_userID() << "'s account." << std::endl;
                return 1;
            }
            current_userID = user_vec[i].get_userID();
            user_index = i;
            std::cout << "User changed to " << user_vec[user_index].get_userID() << "." << std::endl;
            return 1;
        }
    }
    //if no matching user is found, ask if they want to make a new one
    std::cout << "No user found with that ID. Would you like to create user " << uID << " now (y/n)? ";
    bool check = get_user_choice();
    clear_in();

    if (check)
    {
        new_user(uID);
        user_index = user_vec.size() - 1;
        current_userID = user_vec[user_index].get_userID();
        std::cout << "User " << user_vec[user_index].get_userID() << " created." << std::endl;
        std::cout << "Now in " << user_vec[user_index].get_userID() << "'s account." << std::endl;
        return 1;
    }
    else
    {

        std::cout << "Would you like to enter a different user ID (y/n)? ";
        check = get_user_choice();
        clear_in();
        if (check)
        {
            std::cout << "Enter another user ID: ";
            std::cin >> uID;
            change_user(uID);
            return 1;
        }
        else
        {
            std::cout << "User not changed. Current user is still " << user_vec[user_index].get_userID() << std::endl;
            return 0;
        }
    }
}

//follows the user with the passed uID
void Twitter::follow(std::string uID)
{
    if (user_vec.size() == 1)
    {
        std::cout << "There are no other users to follow yet." << std::endl;
        return;
    }
    int i = find_index(uID);
    if (i == -1)
    {
        std::cout << "That user does not exist." << std::endl;
        return;
    }
    user_vec[user_index].add_follow(user_vec[i]);
}

//unfollows the user with the passed uID
void Twitter::unfollow(std::string uID)
{
    if (find_index(uID) == -1)
    {
        std::cout << "That user does not exist." << std::endl;
        return;
    }
    user_vec[user_index].unfollow_user(uID);
    return;
}

//Prints the tweets of each user that the logged in user follows
void Twitter::timeline()
{
    user_vec[user_index].view_timeline();
}


//calls print to file (could probably be one function)
void Twitter::xport(std::string filename, bool for_human)
{
    std::ofstream stream_out;
    stream_out.open(filename);
    if (for_human)
    {
        print_user_tweets(stream_out);
        return;
    }
    create_twitter_file(stream_out);
}

//Calls down through the functions to output a file readable by this program
void Twitter::create_twitter_file(std::ofstream &stream)
{
    bool for_human = false;
    for (int i = 0; i < user_vec.size(); i++)
    {
        if (i != 0)
        {
            stream << "#";
        }
        stream << user_vec[i].get_userID();
        user_vec[i].tweets_to_file(stream, for_human);
    }
}

//prints the tweets of a logged in user to a text document
void Twitter::print_user_tweets(std::ofstream &stream)
{
    bool for_human = true;
    stream << "Here's what you tweeted." << std::endl;
    std::cout << std::endl;
    user_vec[user_index].tweets_to_file(stream, for_human);
}

//updates the index of the current user in the user_vec
int Twitter::find_index(std::string uID)
{
    for (int i = 0; i < user_vec.size(); i++)
    {
        if (uID == user_vec[i].get_userID())
        {
            return i;
        }
    }
    return -1;
}

//gets current user ID using user_index
std::string Twitter::get_current_userID()
{
    current_userID = user_vec[user_index].get_userID();
    return current_userID;
}

//used when loading profile from txt file.
//If duplicate user is seen, new_check_tweet is called instead of new_tweet
void Twitter::write_tweet(std::string uID, std::string twt, std::string dte, bool check_tweets)
{
    if (check_tweets)
    {
        user_vec[user_index].new_tweet_check(uID, twt, dte);
        return;
    }
    user_vec[user_index].new_tweet(uID, twt, dte);
}

//helpers

//accepts a string and sends it to lowercase
std::string str_to_lower(std::string str)
{
    int str_len = str.length();
    for (int i = 0; i < str_len; i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

bool get_user_choice()
{
    std::string user_in;
    std::cin >> user_in;
    user_in = str_to_lower(user_in);
    while (user_in != "y" && user_in != "n" && user_in != "yes" && user_in != "no")
    {
        std::cout << "Please enter either 'yes' or 'no'." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cin >> user_in;
    }

    if (user_in == "y" || user_in == "yes")
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

void clear_in()
{
    std::cin.clear();
    std::cin.ignore(1000, '\n');
}