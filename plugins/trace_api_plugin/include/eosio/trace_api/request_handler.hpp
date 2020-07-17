#pragma once

#include <fc/variant.hpp>
#include <eosio/trace_api/metadata_log.hpp>
#include <eosio/trace_api/data_log.hpp>
#include <eosio/trace_api/common.hpp>

namespace eosio::trace_api {
   using data_handler_function_v0 = std::function<std::tuple<fc::variant, fc::optional<fc::variant>>(const action_trace_v0&, const yield_function&)>;
   using data_handler_function_v1 = std::function<std::tuple<fc::variant, fc::optional<fc::variant>>(const action_trace_v1&, const yield_function&)>;
   
   namespace detail {
      class response_formatter {
      public:
         static fc::variant process_block( const data_log_entry& trace, bool irreversible, const data_handler_function_v0& data_handler_v0, const data_handler_function_v1 & data_handler_v1,    const yield_function& yield );
      };
   }

   template<typename LogfileProvider, typename DataHandlerProvider>
   class request_handler {
   public:
      request_handler(LogfileProvider&& logfile_provider, DataHandlerProvider&& data_handler_provider)
      :logfile_provider(std::move(logfile_provider))
      ,data_handler_provider(std::move(data_handler_provider))
      {
      }

      /**
       * Fetch the trace for a given block height and convert it to a fc::variant for conversion to a final format
       * (eg JSON)
       *
       * @param block_height - the height of the block whose trace is requested
       * @param yield - a yield function to allow cooperation during long running tasks
       * @return a properly formatted variant representing the trace for the given block height if it exists, an
       * empty variant otherwise.
       * @throws yield_exception if a call to `yield` throws.
       * @throws bad_data_exception when there are issues with the underlying data preventing processing.
       */
      fc::variant get_block_trace( uint32_t block_height, const yield_function& yield = {}) {
         auto data = logfile_provider.get_block(block_height, yield);
         if (!data) {
            return {};
         }

         yield();
         
         auto data_handler_v0 = [this](const action_trace_v0& action, const yield_function& yield) -> std::tuple<fc::variant, fc::optional<fc::variant>> {
            return data_handler_provider.template process_data<action_trace_v0>(action, yield);
         };
         auto data_handler_v1 = [this](const action_trace_v1& action, const yield_function& yield) -> std::tuple<fc::variant, fc::optional<fc::variant>> {
            return data_handler_provider.template process_data<action_trace_v1>(action, yield);
         };

         return detail::response_formatter::process_block(std::get<0>(*data), std::get<1>(*data), data_handler_v0, data_handler_v1, yield);
      }

   private:
      LogfileProvider logfile_provider;
      DataHandlerProvider data_handler_provider;
   };


}
