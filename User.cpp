// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"
#define PRINT_USER(os, user) \
    os << "name: <" << user.name_of_user << ">\n" << \
    *(user.recommendation_system) << std::endl;

void User::add_movie_to_rs (const
                            std::string &name, int year, const
                            std::vector<double> &features,
                            double rate)
{
  sp_movie recomm = recommendation_system->add_movie
      (name, year, features);
  ranks_of_user.insert ({recomm, rate});
}

sp_movie User::get_recommendation_by_content () const
{
  sp_movie to_return = recommendation_system->
      get_recommendation_by_content (*this);
  return to_return;
}

sp_movie User::get_recommendation_by_cf (int k) const
{
  sp_movie to_return = recommendation_system->
      recommend_by_cf (*this, k);
  return to_return;
}

double User::get_prediction_score_for_movie (const std::string &name,
                                             int year, int k) const
{
  sp_movie predict = recommendation_system->get_movie (name, year);
  return recommendation_system->predict_movie_score
      (*this, predict, k);
}

std::ostream &operator<< (std::ostream &os, const User &user)
{
  PRINT_USER(os, user);
  return os;
}

