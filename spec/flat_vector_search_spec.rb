# frozen_string_literal: true

RSpec.describe FlatVectorSearch do
  it "has a version number" do
    expect(FlatVectorSearch::VERSION).not_to be_nil
  end

  it "test c++ extension" do
    expect(FlatVectorSearch::TestClass.hello("name")).to eq("Hello world!")
  end
end
