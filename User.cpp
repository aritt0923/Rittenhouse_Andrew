#include "Tweet.h"
#include "User.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Twitter.h"

User::User()
{
    userID = "";
    num_tweets = 0;
}

User::User(std::string uID)
{
    userID = uID;
    num_tweets = 0;
}


//creates a new tweet and adds that tweet to the user's tweet_vec
void User::new_tweet(std::string uID, std::string twt, std::string dte)
{
    Tweet temp(uID, twt, dte);
    tweet_vec.push_back(temp);
    num_tweets++;
}

//same as above, but does not add redundant tweets (used for reading tweets from file)
void User::new_tweet_check(std::string uID, std::string twt, std::string dte)
{
    for (int i = 0; i < tweet_vec.size(); i++)
    {
        if (dte == tweet_vec[i].get_tweet_date() && twt == tweet_vec[i].get_tweet_body())
        {
            return;
        }
    }
    Tweet temp(uID, twt, dte);
    tweet_vec.push_back(temp);
    num_tweets++;
}

//prints user's tweets to screen (used for the timeline function)
void User::print_tweets()
{
    std::cout << std::endl;
    for (int i = 0; i < tweet_vec.size(); i++)
    {
        std::cout << "Tweet #" << i + 1 << std::endl;
        std::cout << tweet_vec[i].to_tweet() << std::endl;
    }
}

/* writes tweets to file. The for_human variable adjusts the formatting
to be human readable if it is set to true, otherwise the function prints 
in a format readable by the program */
void User::tweets_to_file(std::ofstream &stream, bool for_human)
{
    stream << std::endl;
    for (int i = 0; i < tweet_vec.size(); i++)
    {
        if (for_human)
        {
            stream << "Tweet # " << i+1 << std::endl;
            stream << std::endl; 
        }

        stream << tweet_vec[i].to_file(for_human);

        if (for_human)
        {
            stream << std::endl;
        }
    }
}

//follows a user by adding that user to a vector of followed users.
void User::add_follow(User &u)
{
    follow_vec.push_back(u);
    std::cout << "Now following " << follow_vec.back().get_userID() << std::endl;
}

//prints the tweets of every user this user follows
void User::view_timeline()
{
    for (int i = 0; i < follow_vec.size(); i++)
    {
        std::cout << "Here's what " << follow_vec[i].get_userID() << " Tweeted: " << std::endl;
        follow_vec[i].print_tweets();
    }
}

std::string User::get_userID()
{
    return userID;
}

int User::get_num_tweets()
{
    return num_tweets;
}


 // deletes a given element of a given vector
void User::unfollow_user(std::string uID)
{
    int index = find_index_user(uID);
    if(index == -1)
    {
        std::cout << "You are not following that user." << std::endl;
        return;
    }
    for (int i = index+1; i < follow_vec.size(); i++) 
            {
                follow_vec[i-1] = follow_vec[i]; //shift all elements left starting with element to the right of deletion target 
            }
            follow_vec.pop_back(); // pop
    std::cout << "You are no longer following " << uID << std::endl;
}

int User::find_index_user(std::string uID)
{
    for (int i = 0; i < follow_vec.size(); i++)
    {
        if (uID == follow_vec[i].get_userID())
        {
            return i;
        }
    }
    return -1;
}