require 'rubygems'
require '../lib/real_include'
require 'bacon'

describe 'Including a module into a class using real_include' do
  before do
    @m = Module.new {
      def self.class_method
        :class_method
      end

      def instance_method
        :instance_method
      end
    }

    @c = Class.new

    @c.send(:real_include, @m)
  end

  it 'should make class methods accessible to class' do
    @c.class_method.should.equal :class_method
  end

  it 'should make instance methods accessible to instances of the class' do
    obj = @c.new
    obj.instance_method.should.equal :instance_method
  end
end

    
describe 'Including a module into a module and then into a class using real_include' do
    before do
    @m1 = Module.new {
      def self.class_method1
        :class_method1
      end

      def instance_method1
        :instance_method1
      end
    }

    @m2 = Module.new {
      def self.class_method2
        :class_method2
      end

      def instance_method2
        :instance_method2
      end
    }
    @m2.send(:real_include, @m1)

    @c = Class.new

    @c.send(:real_include, @m2)
  end

  it 'should make class methods on m1 accessible to m2' do
    @m2.class_method1.should.equal :class_method1
  end

  it 'should make class methods on modules m1 and m2 accessible to class' do
    @c.class_method1.should.equal :class_method1
    @c.class_method2.should.equal :class_method2
  end

  it 'should make instance methods on modules m1 and m2 accessible to instances of class' do
    obj = @c.new

    obj.instance_method1.should.equal :instance_method1
    obj.instance_method2.should.equal :instance_method2
  end

  it 'should make ancestor chain "look" accurate' do
    @m1 = Module.new {
      def self.class_method1
        :class_method1
      end

      def instance_method1
        :instance_method1
      end
    }

    Object.const_set(:M1, @m1) 

    @m2 = Module.new {
      def self.class_method2
        :class_method2
      end

      def instance_method2
        :instance_method2
      end
    }
    Object.const_set(:M2, @m2) 

    @m2.send(:real_include, @m1)

    @c = Class.new

    @c.send(:real_include, @m2)
    
    @c.ancestors[1].to_s.should.equal M2.name
    @c.class_method1.should.equal :class_method1
    @c.class_method2.should.equal :class_method2
  end

  it 'should work with normal Module#include' do
    @m1 = Module.new {
      def self.class_method1
        :class_method1
      end

      def instance_method1
        :instance_method1
      end
    }

    Object.const_set(:N1, @m1) 

    @m2 = Module.new {
      def self.class_method2
        :class_method2
      end

      def instance_method2
        :instance_method2
      end
    }
    Object.const_set(:N2, @m2) 

    @m2.send(:include, @m1)

    @c = Class.new

    @c.send(:real_include, @m2)
    
    @c.ancestors[1].to_s.should.equal N2.name
    @c.ancestors[2].to_s.should.equal N1.name
    @c.class_method1.should.equal :class_method1
    @c.class_method2.should.equal :class_method2
  end
  
end
