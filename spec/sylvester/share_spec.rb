require 'spec_helper'

describe Sylvester::Share do
  let(:share_params) { TEST_SHARES[0] }
  let(:share_params_2) { TEST_SHARES[1] }

  it "should be a class" do
    Sylvester::Share.should be_a Class
  end

  describe "constructor" do
    it "should return a valid instance of the class when presented with a positive integer key and a positive integer size" do
      share_params[:key].should be_a Integer
      share_params[:size].should be_a Integer
      share = Sylvester::Share.new share_params[:key], share_params[:size]
      share.should be_a Sylvester::Share
      share.destroy!
    end

    it "should throw an argument error when presented with a negative integer key" do
      lambda {
        share = Sylvester::Share.new -123, share_params[:size]
        share.destroy!
      }.should raise_error ArgumentError, "Shared memory key not a positive integer!"
    end

    it "should throw an argument error when presented with a noninteger key" do
      lambda {
        share = Sylvester::Share.new "test", share_params[:size]
        share.destroy!
      }.should raise_error ArgumentError, "Shared memory key not a positive integer!"
    end

    it "should throw an argument error when presented with a negative integer size" do
      lambda {
        share = Sylvester::Share.new share_params[:key], -123
        share.destroy!
      }.should raise_error ArgumentError, "Specified size not a positive integer!"
    end

    it "should be attached if the opts hash is omitted" do
      share = Sylvester::Share.new share_params[:key], share_params[:size]
      share.should be_attached
      share.destroy!
    end

    it "should be attached if the opts hash key :autoattach is set to anything besides nil or false" do
      share = Sylvester::Share.new share_params[:key], share_params[:size], :autoattach => true
      share.should be_attached
      share.destroy!
      share = Sylvester::Share.new share_params_2[:key], share_params_2[:size], :autoattach => "hat"
      share.should be_attached
      share.destroy!
    end

    it "should be unattached if the opts hash key :autoattach is set to false or nil" do
      share = Sylvester::Share.new share_params[:key], share_params[:size], :autoattach => false
      share.should_not be_attached
      share.destroy!
      share = Sylvester::Share.new share_params_2[:key], share_params_2[:size], :autoattach => nil
      share.should_not be_attached
      share.destroy!
    end
  end

  describe "instance methods" do
    describe "attached?" do
      it "should be true if the share is currently attached to the process" do
        share = Sylvester::Share.new share_params[:key], share_params[:size], :autoattach => false
        share.should_not be_attached
        share.attach!
        share.should be_attached
        share.destroy!
      end

      it "should be false if the share is currently not attached to the process" do
        share = Sylvester::Share.new share_params[:key], share_params[:size]
        share.should be_attached
        share.detach!
        share.should_not be_attached
        share.destroy!
      end
    end

    describe "attach!" do
      it "should attach the shared memory to the current process if it isn't already" do
        share = Sylvester::Share.new share_params[:key], share_params[:size], :autoattach => false
        share.attach!
        share.should be_attached
        share.attachment_count.should == 1
        share.destroy!
      end

      it "should do nothing if the share is already attached" do
        share = Sylvester::Share.new share_params[:key], share_params[:size]
        share.should be_attached
        share.attachment_count.should == 1
        share.attach!
        share.should be_attached
        share.attachment_count.should == 1
        share.destroy!
      end
    end

    describe "detach!" do
      it "should detach the shared memory from the current process if it isn't already" do
        share = Sylvester::Share.new share_params[:key], share_params[:size]
        share.detach!
        share.should_not be_attached
        share.attachment_count.should == 0
        share.destroy!
      end

      it "should do nothing if the share is already attached" do
        share = Sylvester::Share.new share_params[:key], share_params[:size], :autoattach => false
        share.should_not be_attached
        share.attachment_count.should == 0
        share.detach!
        share.should_not be_attached
        share.attachment_count.should == 0
        share.destroy!
      end
    end

    describe "destroy!"

    describe "attachment_count" do
      it "should return the number of current attachments" do
        share = Sylvester::Share.new share_params[:key], share_params[:size], :autoattach => false
        share.attachment_count.should == 0
        share.attach!
        share.attachment_count.should == 1
        pid = execute_binary "attach #{share_params[:key]} #{share_params[:size]}"
        sleep 0.1
        share.attachment_count.should == 2
        share.detach!
        share.attachment_count.should == 1
        kill_binary pid
        sleep 0.1
        share.attachment_count.should == 0
        share.destroy!
      end
    end

    describe "key" do
      it "should retrieve the share key" do
        share = Sylvester::Share.new share_params[:key], share_params[:size]
        share.key.should == share_params[:key]
        share.destroy!
      end
    end

    describe "size" do
      it "should retrieve the size of the share" do
        share = Sylvester::Share.new share_params[:key], share_params[:size]
        share.size.should == share_params[:size]
        share.destroy!
      end
    end
  end
end
