# frozen_string_literal: true

RSpec.describe FlatVectorSearch do
  it "has a version number" do
    expect(FlatVectorSearch::VERSION).not_to be_nil
  end

  it "test c++ extension" do
    index = FlatVectorSearch::Index.new(2)
    expect(index.get_n_dims).to eq(2)

    expect(index.add_item("test", [0, 0.1])).to be(true)

    expect(index.get_items).to eq({ "test" => [0.0, 0.1] })

    expect(index.get_item("test")).to eq([0.0, 0.1])
  end
end
