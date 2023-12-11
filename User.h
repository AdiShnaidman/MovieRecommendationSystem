//
// Created on 2/20/2022.
//

#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <string>
#include <memory>
#include "Movie.h"
#include "User.h"
#include <vector>

class RecommendationSystem;

/**
 * A typedef for an unordered_map that maps a shared pointer to a movie
 * (sp_movie) to a double. It uses hash_func and equal_func as the hash
 * and equality functions, respectively.
 */
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;

class User
{
 private:

  /**
   * A private member variable that stores the name of the user.
   */
  std::string name_of_user;

  /**
    * A private member variable that represents the ranks of movies given
    * by the user. It is defined as an unordered_map that maps a shared
    * pointer to a movie (sp_movie) to a double.
    */
  rank_map ranks_of_user;
  /**
  * A private member variable that holds a shared pointer
   * to a RecommendationSystem object.
  */
  std::shared_ptr<RecommendationSystem> recommendation_system;

 public:
  /**
   * Constructor for the class
   */
  // TODO User() this constructor can be implemented however you want
  User (std::string &user_name, rank_map &ranks_of_user,
        std::shared_ptr<RecommendationSystem> &Recommendation_Sys) :
      name_of_user (user_name), ranks_of_user (ranks_of_user),
      recommendation_system (Recommendation_Sys)
  {}

  /**
   * a getter for the user's name
   * @return the username
   */
  // TODO get_name()
  std::string get_name () const
  { return name_of_user; };

  /**
   * function for adding a movie to the DB
   * @param name name of movie
   * @param year year it was made
   * @param features a vector of the movie's features
   * @param rate the user rate for this movie
   */
  void add_movie_to_rs (const
                        std::string &name, int year, const
                        std::vector<double> &features,
                        double rate);

  /**
   * a getter for the ranks map
   * @return
   */
  // TODO get_ranks()
  rank_map get_ranks () const
  { return this->ranks_of_user; };

  /**
   * returns a recommendation according to the movie's content
   * @return recommendation
   */
  sp_movie get_recommendation_by_content () const;

  /**
   * returns a recommendation according to the similarity
   * recommendation method
   * @param k the number of the most similar movies to calculate by
   * @return recommendation
   */
  sp_movie get_recommendation_by_cf (int k) const;

  /**
   * predicts the score for a given movie
   * @param name the name of the movie
   * @param year the year the movie was created
   * @param k the parameter which represents the number
   * of the most similar movies to predict the score by
   * @return predicted score for the given movie
   */
  double
  get_prediction_score_for_movie (const std::string &name,
                                  int year, int k) const;

  /**
   * output stream operator
   * @param os the output stream
   * @param user the user
   * @return output stream
   */
  // TODO &operator<<
  friend std::ostream &operator<< (std::ostream &os, const User &user);
};

#endif //USER_H