//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include <map>
#include "User.h"

/**
 * A structure that defines the comparison operator for comparing
 * shared pointers to movies (sp_movie). It is used as the comparison
 * function for the recommendations_map.
 */
struct movie_cmp
{
    bool operator() (const sp_movie &lhs, const sp_movie &rhs) const
    {
      return (*lhs) < (*rhs);
    }
};

/**
 * Typedef for a std::map that maps a shared pointer to a movie (sp_movie)
 * to a vector of doubles. It uses the movie_cmp struct as the
 * comparison function.
 */
typedef std::map<sp_movie, std::vector<double>, movie_cmp>
    recommendations_map;

class RecommendationSystem
{

 private:

  /**
   * A typedef for a std::map that maps a shared pointer to a movie
   * (sp_movie) to a vector of doubles.
   * It uses the movie_cmp struct as the comparison function.
   */
  recommendations_map mapper;

  /**
   * Member function of the RecommendationSystem class that returns
   * the number of features.
   */
  int num_of_feat (){return (int) mapper.begin ()->second.size ();};

  /**
   * Member function of the RecommendationSystem class that calculates the
   * angle between two vectors represented by std::vector<double>.
   */
  double angle (std::vector<double>& first_vector, std::vector<double>&
  second_vector);

  /**
   * Member function of the RecommendationSystem class that returns
   * a vector of recommendations for a given user.
   * @param user
   * @return
   */
  std::vector<double> get_vector_of_recommendations (User &user);

 public:

  explicit RecommendationSystem() {mapper = recommendations_map{};};
//  RecommendationSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest
   * score based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie get_recommendation_by_content (const User &user);

  /**
   * a function that calculates the movie with highest
   * predicted score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const User &user, int k);

  /**
   * Predict a user rating for a movie given argument
   * using item cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const User &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  // TODO operator<<
  /**
   * Friend function that overloads the << operator to output the contents
   * of a RecommendationSystem object to an output stream.
   */
  friend std::ostream &operator<< (std::ostream &os, const
  RecommendationSystem &recommendation_system);

  /**
   * Function that finds the best content (movie) based on a vector of
   * recommendations and user ranks. It returns a shared pointer to
   * the best content.
   */
  sp_movie best_content_finder(const std::vector<double>&
  vectors_of_recommendations, const rank_map& ranks_holder);
};

#endif //RECOMMENDATIONSYSTEM_H