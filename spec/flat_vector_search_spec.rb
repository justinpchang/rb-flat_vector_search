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

    expect(index.remove_item("test")).to be(true)
    expect(index.remove_item("test")).to be(false)

    expect(index.get_items.keys.length).to eq(0)
  end

  describe "nearest_k" do
    before do
      @index = FlatVectorSearch::Index.new(2)
      @index.add_item("0,1", [0, 1])
      @index.add_item("1,1", [1, 1])
      @index.add_item("1,0", [1, 0])
      @index.add_item("1,-1", [1, -1])
      @index.add_item("0,-1", [0, -1])
    end

    it "nearest_k_by_vector" do
      expect(@index.nearest_k_by_vector([1, 0], 3)).to eq(["1,0", "1,-1", "1,1"])
    end
  end
end
