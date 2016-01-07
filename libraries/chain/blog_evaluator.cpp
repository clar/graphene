#include <graphene/chain/account_object.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/blog_evaluator.hpp>

#include <fc/smart_ref_impl.hpp>
#include <graphene/chain/protocol/fee_schedule.hpp>

#include <fc/uint128.hpp>

namespace graphene { namespace chain {
void_result blog_post_create_evaluator::do_evaluate( const blog_post_create_operation& o )
{
   return void_result();
}
object_id_type blog_post_create_evaluator::do_apply( const blog_post_create_operation& o )
{
   /// TODO: add the data size to the leaky bucket of "Data Rate", perhaps this can be done at
   /// a higher level so it universally applies to all transactions.
   const auto& new_post = db().create<blog_post_object>([&](blog_post_object& obj){
       obj.post = o.post;
       obj.created = db().head_block_time();
       obj.last_update = db().head_block_time();
   });
   return new_post.id;
}

void_result blog_post_update_evaluator::do_evaluate( const blog_post_update_operation& o )
{
   return void_result();
}
void_result blog_post_update_evaluator::do_apply( const blog_post_update_operation& o )
{
   /// TODO: add the data size to the leaky bucket of "Data Rate"
   db().modify( o.post_id(db()), [&](blog_post_object& obj){
       FC_ASSERT( obj.post.author == o.post.author );
       obj.post = o.post;
       obj.last_update = db().head_block_time();
   });
   return void_result();
}

void_result comment_create_evaluator::do_evaluate( const comment_create_operation& o )
{
   return void_result();
}

object_id_type comment_create_evaluator::do_apply( const comment_create_operation& o )
{
   const auto& new_comment = db().create<comment_object>([&](comment_object& obj){
       obj.comment = o.comment;
       obj.created = db().head_block_time();
   });
   return new_comment.id;
}


} } // namespace graphene::chain