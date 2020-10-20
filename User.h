#ifndef USER_H
#define USER_H
#include "Tweet.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/*
User class. Holds tweets. Users are 
only created through Twitter class.
*/
class User
{
public:
    //////// CONSTRUCTORS ////////

    /*
    default constructor
    sets userID to empty string
    sets num_tweets to 0
    sets tweet_vec to 0 elements.
    Not intended for use.
    */
    User();

    /*
    parameterized constructor
    only called by twitter.cpp
    sets userID to user provided string
    sets num_tweets to 0
    */
    User(std::string uID);

    //////// METHODS ////////

    /*
    creates new tweet
    adds tweet to tweet_vec
    called by twitter.cpp
    */
    void new_tweet(std::string uID, std::string twt, std::string dte);

    void new_tweet_check(std::string uID, std::string twt, std::string dte);

    /*
    prints all of a users tweets (loops through tweet_vec
    calls tweet_vec[i].to_tweet();
    called by twitter.cpp
    */
    void print_tweets();

    void tweets_to_file(std::ofstream &stream, bool for_human);

    //adds passed user to follow vec
    void add_follow(User &u);

    void unfollow_user(std::string uID);

    int find_index_user(std::string uID);

    void view_timeline();

   /////// GETTERS ///////

    //returns userID as string
    std::string get_userID();


    //returns num_tweets as int
    int get_num_tweets();
    

private:

std::string userID;
int num_tweets; //number of user tweets
std::vector <Tweet> tweet_vec; //vector of users tweets
std::vector <User> follow_vec; //stores all users that the user follows
std::string password;
};

#endif