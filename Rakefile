require 'rake/extensiontask'

desc 'Run the default task'
task :default => :all

desc 'Run all tasks'
task :all => :compile

Rake::ExtensionTask.new('seal')

rule /^demo:/ do |r|
  Rake::Task[:compile].invoke
  sh 'ruby -I lib -X demo %s.rb' % r.name[/(?<=:).+/]
end

namespace :win32api do
  desc 'Run RSpec using Win32API binding'
  task :rspec do
    sh 'rspec -I win32api -r ./win32api/rspec_config.rb'
  end

  desc 'Concatenate Win32API binding files into one file'
  task :cat do
    open(File.join('win32api', 'concatenated.rb'), 'wb') do |fout|
      content = "module Seal\n"
      %w{
        seal_api
        core
        listener
        buffer
        stream
        reverb
        source
        effect_slot
      }.each do |mod|
        open(File.join('win32api', mod + '.rb'), 'rb') do |fin|
          content += fin.read[/  (class|module).+  end/m]
          content += "\n\n"
        end
      end
      content.rstrip!
      content += "\nend"
      fout << content
    end
  end
end