//
// Created by adish on 6/22/2023.
//
#include <valarray>
#include "RecommendationSystem.h"
#define PRINT_MOVIE(os, movie) os << *(movie) << '\n';
#define MAGIC_NEG_ONE (-(1.0))

static std::vector<double>
scalar_multiplication (const double scalar, const std::vector<double> &vector)
{
  std::vector<double> to_return = vector;
  for (size_t i = 0; i < vector.size (); i++)
  {
    to_return[i] = scalar * vector[i];
  }
  return to_return;
}

std::vector<double>
vector_summing (const std::vector<double> &lhs,
                const std::vector<double> &rhs)
{
  std::vector<double> to_return = lhs;
  for (size_t i = 0; i < lhs.size (); i++)
  {
    to_return[i] += rhs[i];
  }
  return to_return;
}

double dot_product (const std::vector<double> &lhs, const
std::vector<double>
&rhs)
{
  double multiplied = 0;
  size_t i = 0;
  size_t size_of_lhs = lhs.size ();
  while (i < size_of_lhs)
  {
    multiplied += lhs[i] * rhs[i];
    i++;
  }
  return multiplied;
}

sp_movie
RecommendationSystem::best_content_finder (const std::vector<double>
       &vectors_of_recommendations, const rank_map &ranks_holder)
{
  sp_movie best_recommended;
  double highest_rank = MAGIC_NEG_ONE;

  auto it = mapper.begin ();
  while (it != mapper.end ())
  {
    const sp_movie &movie = it->first;
    const std::vector<double> &features = it->second;

    if (ranks_holder.find (movie) == ranks_holder.end ())
    {
      auto &casted_fit = const_cast<std::vector<double> &>(features);
      auto &casted_rec_vec = const_cast<std::vector<double> &>
      (vectors_of_recommendations);
      double rank = angle (casted_fit, casted_rec_vec);

      if (rank >= highest_rank)
      {
        best_recommended = movie;
        highest_rank = rank;
      }
    }
    ++it;
  }
  return best_recommended;
}

sp_movie RecommendationSystem::get_recommendation_by_content
    (const User &user_rankings)
{
  User to_send = const_cast<User &>(user_rankings);
  std::vector<double> vectors_of_recommendations =
      get_vector_of_recommendations (to_send);
  rank_map ranks_holder = user_rankings.get_ranks ();
  return best_content_finder (vectors_of_recommendations, ranks_holder);
}

std::vector<double>
RecommendationSystem::get_vector_of_recommendations (User &user)
{
  rank_map user_ranks = user.get_ranks ();
  double mean = 0.0;
  size_t movie_num = user_ranks.size ();

  for (auto &it: user_ranks)
  {
    mean += it.second;
  }
  mean = mean / static_cast<double>(movie_num);
  double starter_val_for_v = 0.0;
  std::vector<double> vector_of_recommendations (num_of_feat ()
      , starter_val_for_v);
  auto it_ranks = user_ranks.begin ();
  while (it_ranks != user_ranks.end ())
  {
    if (it_ranks->second == 0)
    {
      continue;
    }
    const sp_movie &movie = it_ranks->first;
    double rating = it_ranks->second;
    const std::vector<double> &features = mapper[movie];
    double scalar_to_m = rating - mean;
    const std::vector<double> vector_to_m = features;

    std::vector<double> vec_to_add = scalar_multiplication (scalar_to_m,
                                                            vector_to_m);
    vector_of_recommendations = vector_summing (vec_to_add,
                                                vector_of_recommendations);
    it_ranks++;
  }
  return vector_of_recommendations;
}


double
RecommendationSystem::angle (std::vector<double> &first_vector,
                             std::vector<double> &
                             second_vector)
{
  double norm_of_vec1 = sqrt (dot_product (first_vector,
                                           first_vector));
  double norm_of_vec2 = sqrt (dot_product (second_vector,
                                           second_vector));
  double dot_prod = dot_product (first_vector, second_vector);
  double mult_of_n_denominator = norm_of_vec1 * norm_of_vec2;
  return dot_prod / mult_of_n_denominator;
}

double
RecommendationSystem::predict_movie_score (const User &user,
                                           const sp_movie &movie, int k)
{
  rank_map all_ranks = user.get_ranks ();
  std::map<double, sp_movie> scorer;
  for (auto &it: all_ranks)
  {
    std::vector<double> &first_vector = mapper[movie];
    std::vector<double> &second_vector = mapper[it.first];
    double product = angle (first_vector, second_vector);
    scorer[product] = it.first;
  }
  int k_holder = k;
  auto it = scorer.rbegin ();
  double numerator = 0.0;
  double denominator = 0.0;

  while (k_holder > 0 && it != scorer.rend ())
  {
    double numerator_adder = it->first * all_ranks[it->second];
    numerator += numerator_adder;
    double denominator_adder = it->first;
    denominator += denominator_adder;
    k_holder--;
    it++;
  }
  return numerator / denominator;
}


sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k)
{
  double best_score_holder = 0.0;
  rank_map user_ranks = user.get_ranks();
  auto movie_iterator = mapper.begin();
  sp_movie movie_grabber;
  while (movie_iterator != mapper.end())
  {
    bool still_going = user_ranks.find(movie_iterator->first)
                       == user_ranks.end();
    if (still_going)
    {
      double score_giver = this->predict_movie_score
          (user, movie_iterator->first, k);
      bool higher_score = best_score_holder <= score_giver ;
      if (higher_score)
      {
        movie_grabber = movie_iterator->first;
        best_score_holder = score_giver;
      }
    }
    ++movie_iterator;
  }
  return movie_grabber;
}

sp_movie
RecommendationSystem::add_movie (const std::string &name,
                     int year, const std::vector<double> &features)
{
  sp_movie to_insert = std::make_shared<Movie> (name, year);
  mapper.insert (std::pair<sp_movie, std::vector<double> > ({to_insert,
                                                             features}));
  return to_insert;
}

sp_movie RecommendationSystem::get_movie (const std::string &name, int year)
const
{
  sp_movie movie_giver = std::make_shared<Movie> (name, year);
  auto it = mapper.find (movie_giver);
  sp_movie no_return = nullptr;
  return (it != mapper.end ()) ? it->first : no_return;
}

std::ostream &operator<< (std::ostream &os, const RecommendationSystem &rs)
{
  for (const auto &movie: rs.mapper)
  {
    PRINT_MOVIE(os, movie.first);
  }
  return os;
}
