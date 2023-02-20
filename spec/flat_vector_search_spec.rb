# frozen_string_literal: true

RSpec.describe FlatVectorSearch do
  it "has a version number" do
    expect(FlatVectorSearch::VERSION).not_to be_nil
  end

  it "test c++ extension" do
    index = FlatVectorSearch::Index.new(2)
    expect(index.get_n_dims).to eq(2)

    expect(index.add_item(0, [0, 0.1])).to be true
  end
end
