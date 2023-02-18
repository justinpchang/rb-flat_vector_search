# frozen_string_literal: true

require_relative "lib/flat_vector_search/version"

Gem::Specification.new do |spec|
  spec.name = "flat_vector_search"
  spec.version = FlatVectorSearch::VERSION
  spec.authors = ["Justin Chang"]
  spec.email = ["justin.p.chang@gmail.com"]

  spec.summary = "Fast, brute force vector similarity search"
  spec.homepage = "https://github.com/justinpchang/rb-flat_vector_search"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 2.6.0"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage
  spec.metadata["changelog_uri"] = "https://github.com/justinpchang/rb-flat_vector_search/blob/main/CHANGELOG.md"
  spec.metadata["documentation_uri"] = "https://justinpchang.github.io/rb-flat_vector_search/doc/"
  spec.metadata["rubygems_mfa_required"] = "true"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(__dir__) do
    `git ls-files -z`.split("\x0").reject do |f|
      (f == __FILE__) || f.match(%r{\A(?:(?:bin|test|spec|features)/|\.(?:git|travis|circleci)|appveyor)})
    end
  end

  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions = ["ext/flat_vector_search/extconf.rb"]
end
