/*Main class for Twitter project
Classes: Twitter, User, Tweet 
Twitter holds a vector of all users
users hold vectors of tweets and other
users that they follow*/

#ifndef TWITTER_H
#define TWITTER_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "User.h"
#include "Tweet.h"

class Twitter
{
public:
    //////// CONSTRUCTORS ////////

    /* default constructor
    sets user_vec to 0 elements */
    Twitter();

    /* parameterized constructor, accepts user ID, creates that user, 
    adds user to user_vec, sets user_index to 0 */
    Twitter(std::string uID);

    /* parameterized constructor
    creates twitter object and users from passed file */
    Twitter(std::ifstream &user_file);

    void from_file(std::ifstream &fin);

    void user_reader(std::stringstream &ss);

    void tweet_reader(std::stringstream &ss, bool check_tweets);

    //////// METHODS ////////

    // creates new user, adds user to user_vec
    void new_user(std::string uID);

    bool new_user_fin(std::string uID);

    /* loops through user_vec and returns the matching index
    not sure I ever used this */
    int find_index(std::string uID);

    //adds user to follow_vec of current account
    void follow(std::string uID);

    //unfollow a user
    void unfollow(std::string uID);

    //calls view_timeline with current user
    void timeline();

    //calls new_tweet with current user
    void write_tweet(std::string uID, std::string twt, std::string dte);

    //calls new_tweet_check if check_tweet is true
    void write_tweet(std::string uID, std::string twt, std::string dte, bool check_tweets);

    //changes user or prompts to create new user
    bool change_user(std::string uID);

    //uses user_index to find current uID
    std::string get_current_userID();

    //creates file that can be read back in to program
    void xport(std::string filename, bool for_human);

    //prints everyone's tweets
    void create_twitter_file(std::ofstream &stream);

    void print_user_tweets(std::ofstream &stream);

private:
    std::vector<User> user_vec; //stores all users
    int user_index;             //index of current_user in user_vec
    std::string current_userID; //ID that user logged in with
};

#endif