# frozen_string_literal: true

RSpec.describe FlatVectorSearch do
  it "has a version number" do
    expect(FlatVectorSearch::VERSION).not_to be_nil
  end

  it "test c++ extension" do
    index = FlatVectorSearch::Index.new(5)
    expect(index.get_n_dims).to eq(5)
  end
end
