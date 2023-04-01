#pragma once

enum Error_code {
	success, fail, utility_range_error, underflow, overflow, fatal,
	not_present, duplicate_error, entry_inserted, entry_found,
	internal_error
};

void initialize(int &end_time, int &queue_limit,
                double &arrival_rate, double &departure_rate);

void run_idle(int time);