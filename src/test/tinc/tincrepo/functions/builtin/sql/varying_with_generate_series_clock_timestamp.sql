-- @description varying_with_generate_series_clock_timestamp.sql
-- @db_name builtin_functionproperty
-- @author tungs1
-- @modified 2013-04-17 12:00:00
-- @created 2013-04-17 12:00:00
-- @executemode ORCA_PLANNER_DIFF
-- @tags functionPropertiesBuiltin HAWQ
SELECT count( distinct i ) FROM (SELECT clock_timestamp() i, generate_series(1, 5) j) t;
