require 'spec_helper'

describe Sylvester::ByteArrayShare do
  let(:share_params) { TEST_SHARES[0] }
  let(:share_params_2) { TEST_SHARES[1] }

  it "should be a class" do
    Sylvester::ByteArrayShare.should be_a Class
  end

  it "should be a subclass of Sylvester::Share" do
    Sylvester::ByteArrayShare.superclass.should == Sylvester::Share
  end

  describe "instance methods" do
    describe "length" do
      it "should return the length of the bytearray" do
        share = Sylvester::ByteArrayShare.new share_params[:key], share_params[:size]
        share.length.should == 0
        share << 0xff
        share.length.should == 1
        share.destroy!
      end
    end

    describe "last" do
      it "should return the last element of the bytearray" do
      end
    end

    describe "<<"
    describe "push"
  end
end
