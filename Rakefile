# frozen_string_literal: true

require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

require "rubocop/rake_task"

RuboCop::RakeTask.new

require "rake/extensiontask"

desc "Build"
task build: :compile

Rake::ExtensionTask.new("flat_vector_search") do |ext|
  ext.ext_dir = "ext/flat_vector_search"
  ext.lib_dir = "lib/flat_vector_search"
end

task default: %i[clobber compile rubocop spec]
